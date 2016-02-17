/*

Sage Ridge Robotics
Example 25

Multi-sensor BattleBot. This BattleBot uses two sensors to try to locate the
enemy. 

Christopher von Nagy

*/

// LOAD LIBRARIES ---------------------------------------------------------------------------------------------

// Adafruit MotorShield
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

// avr math library
#include <math.h>

// Variables ---------------------------------------------------------------------------------------------
const int leftMotorPort  = 1; 
const int rightMotorPort = 2;
const int motorInterval = 5000;


// Instantiate! ------------------------------------------------------------------------------------------
Adafruit_MotorShield AFMS = Adafruit_MotorShield();                    // Drive subsystem
Adafruit_DCMotor *myLeftDCMotor = AFMS.getMotor(leftMotorPort);
Adafruit_DCMotor *myRightDCMotor = AFMS.getMotor(rightMotorPort);

// Setting things up, including the interrupt
void setup()  
{
    
  Serial.begin(115200);
  Serial.println("Start setup . . . ");
  
  
  Serial.println("Motors ");
  AFMS.begin();
  
  Serial.println(" . . . Setup complete");
  
}

//The main loop -----------------------------------------------------------------------------------------
void loop(){
  
 full_stop();
 
}

void driveForward () {
  Serial.println("Driving ... "); 
  myLeftDCMotor->setSpeed(250); 
  myRightDCMotor->setSpeed(250);
  myLeftDCMotor->run(FORWARD); 
  myRightDCMotor->run(FORWARD); 
  delay(motorInterval);
}

void rotate_to_f_azimuth (float a_correction) {
  
  int iterations = floor(abs(a_correction));
  int d_period = 20;
  myLeftDCMotor->setSpeed(100); 
  myRightDCMotor->setSpeed(100);
  
  if ( a_correction < 0 ) {
    Serial.print("Correcting left "); 
    Serial.print(iterations);
    Serial.print(" degrees ");
    for (int i=0; i < iterations; i++) {
      Serial.print(".");
      myLeftDCMotor->run(BACKWARD); 
      myRightDCMotor->run(FORWARD);
      delay(d_period);
    }
    myLeftDCMotor->run(RELEASE); 
    myRightDCMotor->run(RELEASE);
    Serial.println(" done"); 
  } 
  
  if ( a_correction > 0 ) {
    Serial.print("Correcting right "); 
    Serial.print(iterations);
    Serial.print(" degrees ");
    for (int i=0; i < iterations; i++) {     
      Serial.print(".");
      myLeftDCMotor->run(FORWARD); 
      myRightDCMotor->run(BACKWARD);
      delay(d_period);
    }
    myLeftDCMotor->run(RELEASE); 
    myRightDCMotor->run(RELEASE);
    Serial.println(" done"); 
  }   
}

void full_stop() {
       myLeftDCMotor->run(RELEASE); 
      myRightDCMotor->run(RELEASE);
}

