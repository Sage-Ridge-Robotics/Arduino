/*

Sage Ridge Robotics
Example 22

Color Bot II. 

This example sketch integrates a luminosity, ultrasonic distance, 
and two (2) color sensors to drive a single steering servo and two motors 
allowing a robot to follow a color-marked course.

** Caution, There is a bug in the library so this sketch currently does not work **

This sketch is modified from the example sketch published by Adafruit, LLC 
on github. This code is in the public domain.

In order to use two color sensors with hard-coded I2C addresses (these would conflict on 
a single I2C bus), we use an alternative library to designate arbitrary Arduino pins as the SCL 
and SDA pins. This allows us to create multiple I2C busses. 

This Sketch requires the SRS_Robotics_Multisensor_TCS34725 library.

*/

// LOAD LIBRARIES ---------------------------------------------------------------------------------------------

// Sensing transducers
// Sensor library and lux sensor library
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h> 

// Color sensor library
// #include "Adafruit_TCS34725.h"                     // The standard TCS34725 library from Adafruit
#include "SRS_Robotics_Multisensor_TCS34725.h"        // This library allows for assigned I2C pins
#include "SRS_SoftI2CMaster.h"                        // Required for the TCS34725.h library

// Motion transducers
#include <Servo.h>                                    // Allows for servo control
#include <Adafruit_MotorShield.h>                     // Allws for the use of the MotorShield
#include "utility/Adafruit_PWMServoDriver.h"

// Sensor functions
#include "./sensor_functions.h"                       // Sensor functions defined for this project

// Motor functions
#include "./motor_functions.h"                        // Motor functions defined for this project

// DECLARE VARIABLES ------------------------------------------------------------------------------------------

// Declare the target color here
// The target color is characterized here by its RGB values. These can be measured using one of the
// color sensors and code that streams the value to the serial monitor. Arduino Code Example 23
// can be loaded onto an Arduino with a single Adafruit_TCS34725 color device attached. This
// sketch will turn the light on, calculate an averaged color based on ten samples and returning
// the samples RGB and LAB values. You could also use Mathematica to calculate a LAB value:
// ColorConvert[RGBColor[0.9, 0.1, 0.1], "LAB"]
const float L_target = 0.50;                               // Target LAB (a red) 
const float a_target = 0.72;
const float b_target = 0.57;

// deltaE threshold. According to the folks who study color, a deltaE of 1.0 means that two colors 
// will seem the same to a human observer. Recent work has suggested slightly larger figures. 
// Lighting conditions matter a great deal. We want to give our robot some flexibility in 
// recognizing the color of the line it is following as the "same" despite different lighting
// coniditions. You will have to play with this threshold. I suggest 5 as a starting point.
const int deltaE_threshold = 5;

// Illumination threshold in lux to trigger lights. Light levels below this threshold will result in
// the robot's "headlights" being turned on. Experiment here. You may get better color matching
// if you set this threshold to zero causing the robot to keep its headlights on.
const int lux_threshold = 50;

// Digital pin assignments
const int tcs01_SDA = 3;           // TCS34725 I2C pin definitions (Color sensors)
const int tcs01_SCL = 4;           // digital pin numbers
const int tcs02_SDA = 5;
const int tcs02_SCL = 6;

const int pingpin = 10;            // "Ping" pin for ultrasonic distance sensor

const int headlight01 = 11;        // Left headlight control pin;
const int headlight02 = 12;        // Right headlight control pin;

const int status_LED = 13;         // This is the on-board LED used for startup diagnostics

// Analog pins
const int vpin = 0;                // Analog voltage pin for ultrasonic distance sensor

// Motor control
const int servoPin = 9;            // Set servo pin

const int motorPort01 = 1;         // Set left motor output on MotorShield. 
const int motorPort02 = 2;         // Set right motor output on MotorShield.

const int motor_speed = 100;       // We set a uniform motor speed.
const int motor_period = 1000;     // We set a uniform delay for motor functions

int headlight = 0;                 // We use headlight to switch the color sensor headlight status
int fstop = 0;                     // We use full stop for collision avoidance.
int sonar_critical_distance = 10;  // The distance in cm at which the sonar function should trigger
                                   // an obstacle routine.

int left_color_status = 0;         // We set left and right color status to an initial 0. A
int right_color_status = 0;        // color status of 1 means a "match" between a sampled color
                                   // reading and the target color.


// INSTATIATE OBJECTS -----------------------------------------------------------------------------------------

// Instantiate an instance of the TSL2561 class
// This allows our sketch to talk to the lux sensor
Adafruit_TSL2561_Unified luxs = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);

// Instantiate instances of the TCS34725 class. Here, we use Linbeck's simplified version of the
// Adafruit library which permits us to specify alternative I2C SCL and SDA pins to support
// multiple instances of this device.
// Adafruit_TCS34725 tcs01 = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X);
// Adafruit_TCS34725 tcs02 = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X);
TCS34725 tcs01;
TCS34725 tcs02;

// Instantiate objects of the servo, Adafruit_MotorShield, and Adafruit_DCMotor classes.
Servo myServo0;   
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *myDCMotor01 = AFMS.getMotor(motorPort01);
Adafruit_DCMotor *myDCMotor02 = AFMS.getMotor(motorPort02);

// SETUP ------------------------------------------------------------------------------------------------------

// Initialize the robot and check that sensors, motors, and servos are working.
// Setup will take several seconds as sensors are activated.

void setup() {
  
  // Initialize sensor transducers
  pinMode(tcs01_SDA, OUTPUT);                         // Setup I2C color sensor pins
  pinMode(tcs01_SCL, OUTPUT);                         // for each sensor 
  pinMode(tcs02_SDA, OUTPUT);
  pinMode(tcs02_SCL, OUTPUT);
  
  pinMode(headlight01, OUTPUT);                       //  Setup headlight pins for output
  pinMode(headlight02, OUTPUT);
  
  pinMode(status_LED, OUTPUT);
  pinMode(pingpin, OUTPUT);
  
  // Initialize sensors  
  init_lux_sensor(&luxs, status_LED);                 // Initialize lux sensor
                                                      // five short blinks indicate success
                                                      // one long blink indicates failure

  init_color_sensor(&tcs01, tcs01_SDA, tcs01_SCL, status_LED);    // Initialize the color sensors
  init_color_sensor(&tcs02, tcs02_SDA, tcs02_SCL, status_LED);    // five short blinks indicate success
                                                      // one long blink indicates failure

  // Initialize transducers
  myServo0.attach(9);
  AFMS.begin();
  myDCMotor01->setSpeed(motor_speed);
  myDCMotor02->setSpeed(motor_speed);

}

// LOOP  ------------------------------------------------------------------------------------------------------

// This loop applies to a robot with two parallel color sensors. It is rather simple. We check to see if
// ambient light levels call for turning on the lights and then turn them on or off. We see if there is an 
// ostacle to be avoided and react accordingly. Then we check the left (tcs01) and right (tcs02) color
// sensors. If a sensor detect the target color, it sends a redirection command (executing a
// soft turn to recenter the color line between the sensors. In this version, there is no search 
// algorithm in the case the robot gets lost. If the robot is going too fast, it may overrun the color
// before reacting.

void loop() {
  
  int headlight = check_light_level(&luxs, lux_threshold);
  switch_headlight(headlight, headlight01, headlight02);
  int fstop = sonar(sonar_critical_distance);
  if (fstop) {avoid_obstacle();}
  
  if (check_color_status(tcs01, LAB_target, deltaE_threshold)) {move_right_soft();}
  if (check_color_status(tcs02, LAB_target, deltaE_threshold)) {move_left_soft(); }
  move_forward();
  
  
  // place search algorithm here?
  // if (some condition of being lost) then ...
  
}

// FUNCTIONS --------------------------------------------------------------------------------------------------

// Motor functions are in motor_functions_h
// Sensor functions are in sensor_functions.h


