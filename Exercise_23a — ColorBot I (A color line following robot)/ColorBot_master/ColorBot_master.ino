/*

Sage Ridge Robotics
Exercise 23a

Color Bot I. 

This Sketch is the primary color sensing system for a color line following robot that uses a second 
Arduino to process and return color data.

This sketch provides the core logic of the robot and depends on the color sensing subsystem.

Stuart Hayes  (w. corrections by C v Nagy)

*/

// LOAD LIBRARIES ---------------------------------------------------------------------------------------------

// Fundamental libraries
#include <Wire.h>

// Motion transducers
#include "Servo.h"
#include "Adafruit_MotorShield.h"
#include "utility/Adafruit_MS_PWMServoDriver.h"

// Sensor functions
// The sonar() fucntion is included in this file rather than sensor.h
//#include "./sensor.h"

// DECLARE VARIABLES ------------------------------------------------------------------------------------------

// Ultrasonic collision avoidance
const int pingpin = 10;            // "Ping" pin for ultrasonic distance sensor
const int vpin = 0;                // Analog voltage pin for ultrasonic distance sensor
int fstop = 0;                     // We use fstop for collision avoidance.
int sonar_critical_distance = 10;  // The distance in cm at which the sonar function should trigger
                                   // an obstacle routine.

// Color subsystem connections
const int opin = 12;               // Receives digital HIGH if the color is sensed
const int spin = 11;               // Receives digital HIGH if the color system is operative

// Motor control
const int motorPort01 = 1;         // Set left motor output on MotorShield. 
const int motorPort02 = 4;         // Set right motor output on MotorShield.

const int motor_speed = 150;       // We set a uniform motor speed.
const int motor_period = 1000;     // We set a uniform delay for motor functions

                                   
int servoangle = 500;              // Initial servo angle 
int boomangle = 90;                // Angle of boom at start
int steeringangle = 90;            // Initial steering angle 



// INSTATIATE OBJECTS -----------------------------------------------------------------------------------------

// Instantiate objects of the servo, Adafruit_MotorShield, and Adafruit_DCMotor classes.
Servo myServo0;
Servo myServo1;
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *myDCMotor01 = AFMS.getMotor(1);
Adafruit_DCMotor *myDCMotor02 = AFMS.getMotor(4);

// SETUP ------------------------------------------------------------------------------------------------------

// Initialize the robot and check that sensors, motors, and servos are working.
// Setup will take several seconds as sensors are activated.

void setup() {
  
  delay(2000);                                                      // Allow the slave Arduino to initialize
  Serial.begin(9600);                                               // For status messages
  
  // Inputs to listen to color subsystem states
  pinMode(opin, INPUT);
  pinMode(spin, INPUT);

  // Initialize motor system
  AFMS.begin();
  myServo0.attach(9);
  myServo1.attach(9);

  // Test and position servo0
  myServo0.write(0);
  delay(1000);
  myServo0.write(180);
  delay(1000);
  myServo0.write(steeringangle);
  delay(1000);

  // Test and position servo1
  myServo1.write(0);
  delay(1000);
  myServo1.write(180);
  delay(1000);
  myServo1.write(boomangle);
  delay(1000);
  
  // Set initial motor parameters
  myDCMotor02->setSpeed(motor_speed);
  myDCMotor01->setSpeed(motor_speed);
  
  // Check slave color subsystem Arduino 
  boolean test = digitalRead(spin);
  Serial.print("Slave Arduino ready? ");
  Serial.print(test); 
  if(!test){Serial.println(" No color system present."); while(1);} // Stop if we do not receive digital HIGH from
                                                                    // color subsystem.
  Serial.println(" All good.");

}

// LOOP  ------------------------------------------------------------------------------------------------------

void loop() {
    
  // Check for obstacle
  int fstop = sonar(sonar_critical_distance, pingpin, vpin);
  if (fstop) {Serial.println("Obstacle"); avoid_obstacle();} else {Serial.println("No obstacle. Continuing.");}
  
  // Check color detection subsystem
  boolean cstatus = digitalRead(opin);
  
  // Move normally if on-color or initialize color detection sweep if not.
  if (cstatus)
     {Serial.println("Color detected. Moving forward normally"); move_forward(steeringangle);} 
  else {
    Serial.println("Color lost. Boom start.");
    for(int i = 0; i < 180; i = i + 5){
        myServo1.write(i);
        delay(1000);
        boolean cstatus = digitalRead(opin);
      if(cstatus){servoangle = (180 - i); break;}
    }   
  }
  if (servoangle = 500){ searchandrescue(); }
  if (servoangle < 500){
    myServo1.write(servoangle);
    myServo0.write(90);
    delay(500);
    myDCMotor01->setSpeed(motor_speed);
    myDCMotor02->setSpeed(motor_speed); 
    myDCMotor01->run(FORWARD);
    myDCMotor02->run(FORWARD);      
    delay(1000);
    servoangle = 90;
    myServo1.write(servoangle);
  }
}

void avoid_obstacle() 
{
  
  myDCMotor01->setSpeed(0);
  myDCMotor02->setSpeed(0); 
  myDCMotor01->run(RELEASE);
  myDCMotor02->run(RELEASE);
  myServo1.write(80);
  myDCMotor01->setSpeed(75);
  myDCMotor02->setSpeed(75); 
  myDCMotor01->run(BACKWARD);
  myDCMotor02->run(BACKWARD);
  delay(500);
  myServo1.write(90);
  myDCMotor01->run(FORWARD);
  myDCMotor02->run(FORWARD);
  delay(1000);
  myServo1.write(105);
  myDCMotor01->setSpeed(75);
  myDCMotor02->setSpeed(75); 
  myDCMotor01->run(FORWARD);
  myDCMotor02->run(FORWARD);
  delay(1000);
  myServo1.write(90);
  myDCMotor01->setSpeed(75);
  myDCMotor02->setSpeed(75); 
  myDCMotor01->run(FORWARD);
  myDCMotor02->run(FORWARD);
}

void move_forward(int angle) 
{
  myServo1.write(angle);
  myDCMotor01->setSpeed(150);
  myDCMotor02->setSpeed(150); 
  myDCMotor01->run(FORWARD);
  myDCMotor02->run(FORWARD);
  delay(2000);

}


void searchandrescue()
{
    servoangle = 500;
    while(servoangle = 500) {
      myDCMotor01->run(RELEASE);
      myDCMotor02->run(RELEASE);
      myDCMotor01->setSpeed(75);
      myDCMotor02->setSpeed(75); 
      myDCMotor01->run(BACKWARD);
      myDCMotor02->run(BACKWARD);
      delay(100);
      myDCMotor01->run(RELEASE);
      myDCMotor02->run(RELEASE);
      for(int i = 0; i < 181; i+5){
          myServo0.write(i);
          delay(500);
          if(digitalRead(opin)){servoangle = (180 - i); break;
        }
          if (servoangle < 500){
            myServo1.write(servoangle);
            myServo0.write(90);
            delay(500);
            myDCMotor01->setSpeed(motor_speed);
            myDCMotor02->setSpeed(motor_speed); 
            while(!digitalRead(opin)){
              myDCMotor01->run(FORWARD);
              myDCMotor02->run(FORWARD);      
            }
            servoangle = 90;
            myServo1.write(servoangle);
          }   
       }
   }
}

// Read sonar function. Returns 1 if the sonar detects an object 
int sonar(int critical_distance, int pingpin, int vpin){

    int voltage = 0;
    float distance;
    int fstop = 0;
    
    pinMode(pingpin,OUTPUT);
    pinMode(vpin,INPUT);
    
    // Calibration
    digitalWrite(pingpin, HIGH);
    delay(100);
    digitalWrite(pingpin, LOW);
    
    // Measure distance as an average of 5 pings
    digitalWrite(pingpin, HIGH);
    for (int i = 0; i < 5; i++){
      voltage += analogRead(vpin);
      delay(50);
    }
    distance = (voltage/5) * 1.153846154;
    if (distance <= critical_distance) fstop = 1;
    return fstop;
}
