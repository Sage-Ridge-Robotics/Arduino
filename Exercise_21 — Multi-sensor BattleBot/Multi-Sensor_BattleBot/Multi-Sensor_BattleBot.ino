/*

Sage Ridge Robotics
Example 21

Multi-sensor BattleBot or sumo robot. This sketch implements a simple proximity approach to using 
two sensors, here utrasonic sonar detectors, to locate and engage an opponent robot. The sensors 
are mounted on opposite sides of a boom mounted to the front of the robot. The goal is to flip the 
opponent. A downward facing lux sensor is used to keep the robot in a "wrestling" ring demarcaded 
with contrastive material.

The sketch is divided into four files, a main file, a file containing motor functions, a file containing
sensor functions, and a header file with macros. The motor and sensor files have no suffix and are
concatenated at the end of the main file prior to compile. The header file is "included" below.

Modify as you see fit to obtimize your robot! Also, modify to optimize the code footprint.
There are a number of places you can remove extraneous code for a smaller memory footprint, 
including removing debugging info sent via the serial link. You might add test routines to
functions that initialize sensors returning a boolean value of true for success and false 
for a problem.

You might consider studying "Exercise_26 — Compass and Accelerometer" to add the ability to detect an
unfriendly collision and to react accordingly.

Copyright (c) 2016 Christopher von Nagy
This code is in the public domain.

*/

// Load Libraries ----------------------------------------------------------------------------------------

// avr math library
#include <math.h>

// Required Wire library
#include <Wire.h>

// Adafruit lux sensor
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h> 

// Adafruit MotorShield
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

// Arduino pin assignments
#include "./pin_layout.h"

// Declare variables -------------------------------------------------------------------------------------

// For pin assignments see pin_layout.h

// Sensor subsystems
// LUX_T is defined as a macro in pin_layout.h       // Lux detection threshold. Adjust if necessary.
// DIST_T is defined as a macro in pin_layout.h      // Maximum distance of a target. If a target is beyond the 
                                                     // diameter of the field, then ignore it. This leaves 
                                                     // the problem of the robot considering folks around the
                                                     // table targets.
                                                      
boolean target_detected;                             // Returns true if a target has been acquired, else false.
boolean target_proximate;                            // Returns true if a target is closer than 10 centimeters.
int target_position;                                 // Returns -1 for left, 0 to dead straight, 1 for right.
int lux_threshold = LUX_T;                           // Initializes lux_threshold to the value specified in the header file.
                                                     // The detected lux value must be equal to or greater than the play surface
                                                     // value plus the lux_threshold value for the system to decide that
                                                     // it has detected the boundary.
                                                     
// Instantiate! ------------------------------------------------------------------------------------------

// lux sensor
Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345); 

// Drive subsystem
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *leftMotor = AFMS.getMotor(LMPORT);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(RMPORT);

// Setup -------------------------------------------------------------------------------------------------

void setup()  
{
  
  // Seed random number generator
  randomSeed(analogRead(0));
  
  // Set pins to INPUT or OUPUT
  pinMode(PPIN0, OUTPUT);
  pinMode(PPIN1, OUTPUT);
  pinMode(VPIN0, INPUT);
  pinMode(VPIN1, INPUT);
  
  digitalWrite(PPIN0, LOW);
  digitalWrite(PPIN1, LOW);


  // Starting the serial interface for monitoring  
  Serial.begin(9600);
  Serial.println("Start setup . . . ");
  
  // Start motor interface. You could add a test of some kind to the function to return false for a problem.
  // while(1) stops execution of the sketch if start_motor_shield() returns false.
  if ( ! start_motor_shield() ) { while(1); }

  // Calibrate the sonars.  You could add a test of some kind to the function to return false for a problem.
  if ( ! calibrate_sonars() ) { while(1); }

  // Turn on lux sensor, configure, and determine brightness of play surface. tsl.begin() in the function 
  // will return false if a problem initializing the sensor occurs.
  if ( ! start_lux_sensor() ) { while(1); } else { configure_lux_sensor(); lux_threshold += detect_play_surface_lux(); }
  
  
  Serial.println(" . . . Setup complete");
  
}

//The main loop -----------------------------------------------------------------------------------------
void loop() {
  
  // Check for the boundary marker. If found, back up a bit. The round is lost if the robot exits the
  // playing field by crossing the boundary tape.
  if ( detect_boundary() ) { full_stop(); driveBackward(); }
  
  // Ping for target if in the main loop.
  target_detect(); 
  
  // If a target has not been detected, search for a target clockwise or anti-clockwise
  if ( ! target_detected ) { 
    int spinDirection = round(random(0,1)); 
    search(spinDirection); 
  }
    
  // If a target still has not been detected, then move forward.
  if ( ! target_detected ) { driveForward(); }
  
  // If a target has been detected and is further than 10 centimeters away, then follow it. 
  if ( target_detected && ! target_proximate ) { follow_target(); }
  
  // If a target is closer than 10 centimeters, lunge at full speed.
  if ( target_detected && target_proximate ) { lunge(); }
  
}
 
   


