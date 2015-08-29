/*

Sage Ridge Robotics
Exercise 11

Controlling multiple DC motors with the Arduino Motor Shield. Motor run
and speed commands are hard-coded into the main loop() function of the
sketch. This is one possible solution. See Example 12 for another solution.

Code adapted from Adafruit Industries, LLC.
This code is in the public domain.

*/

// Load MotorShield libraries. These must be previously installed in your
// personal Arduino/Libraries folder. 
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"
 
// Declare variables used globally in the Sketch.  
// dWait and dRUN are used in delay() in the main loop of the program.
int motorPort01 = 1;
int motorPort02 = 2;

int dWait = 1000;
int dRUN = 4000;

int runSwitch = 1;

// Instantiate an object of the Adafruit_MotorShield class 
// to control a physical servo motor. 
Adafruit_MotorShield AFMS = Adafruit_MotorShield();  

// Instantiate objects of the DC Motor class
Adafruit_DCMotor *myDCMotor01 = AFMS.getMotor(motorPort01);
Adafruit_DCMotor *myDCMotor02 = AFMS.getMotor(motorPort02);
 
 
// Call the begin() method of the MotorShield object to
// ready the object for commands and set the default speed
// of the motors. Speed ranges from 0 to 255. The AFMS object
// begin() method takes an option argument that spcifies the
// maximum frequency. The default is 16 KHz.
void setup() 
{ 
  AFMS.begin();
  myDCMotor01->setSpeed(100);
  myDCMotor02->setSpeed(100);  
} 

// Run the motors using the run() method. The run() method takes one 
// argument: FORWARD, BACKWARD, RELEASE. RELEASE stops the motor.
// The motor command codes will execute while runSwitch is equal to 1.
void loop() 
{
  while (runSwitch == 1){
  
    // Motor commands
    myDCMotor01->run(FORWARD);
    myDCMotor02->run(FORWARD);  
    delay(dRUN);
    myDCMotor01->run(RELEASE);
    myDCMotor02->run(RELEASE);
    delay(dWait);
    myDCMotor01->run(BACKWARD);
    myDCMotor02->run(BACKWARD);
    delay(dRUN);
    myDCMotor01->run(RELEASE);  
    myDCMotor02->run(RELEASE);  
    delay(dWait);
    myDCMotor01->setSpeed(200);
    myDCMotor02->setSpeed(200);    
    myDCMotor01->run(FORWARD);
    myDCMotor02->run(BACKWARD);  
    delay(dRUN);
    myDCMotor01->run(RELEASE);  
    myDCMotor02->run(RELEASE);  
    delay(dWait);
    myDCMotor01->setSpeed(10);
    myDCMotor02->setSpeed(10);    
    myDCMotor01->run(BACKWARD);
    myDCMotor02->run(FORWARD);  
    delay(dRUN);
    
    // Set the run condition to false and stop the motors
    runSwitch = 0; 
    myDCMotor01->run(RELEASE);  
    myDCMotor02->run(RELEASE);
  }
    delay(1000);  
}




