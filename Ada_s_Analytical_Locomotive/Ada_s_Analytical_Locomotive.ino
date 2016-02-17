
/*
Sage Ridge Robotics
Mathew, Michael, Dr. Chris
 
Code adapted from Adafruit Industries, LLC.
This code is in the public domain.
 
*/
 
// Load MotorShield libraries. These must be previously installed in your
// personal Arduino/Libraries folder. 
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"
#include <Servo.h>

// Declare variables used globally in the Sketch. 
// dWait and dRUN are used in delay() in the main loop of the program.
int motorPort01 = 1;
int motorPort02 = 4;
 
// Instantiate an object of the Adafruit_MotorShield class 
// to control a physical servo motor. 
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Connect a servo to the Motor Shield servo pins 
// and instantiate a servo object. 
Servo servo1;

 
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
   servo1.attach(10);
  
   // Motor commands
   myDCMotor01->setSpeed(100);
   myDCMotor02->setSpeed(100);
   myDCMotor01->run(FORWARD);
   myDCMotor02->run(FORWARD); 

   // go straight
   servo1.write(95);
   delay(2000);
   
   // 1 turn right
   servo1.write(150);
   delay(1500);
   
   //go straight
   servo1.write(95);
   delay(1500);
   
   // 2 turn left
   servo1.write(40);
   delay(1500);
   
   //go straight
   servo1.write(95);
   delay(750);
   
   // 3 turn right
   servo1.write(160);
   delay(1500);
   
   // go straight
   servo1.write(95);
   delay(1500);
   
   // 4 turn left
   servo1.write(40);
   delay(1500);
   
   // go straight 
   servo1.write(95);
   delay(1500);
   
   // 5 go left
   servo1.write(30);
   delay(1500);
   
   // go straight
   servo1.write(95);
   delay(3750);
   
   // 6 go right
   servo1.write(167);
   delay(1500);
   
   // go straight
   servo1.write(95);
   delay(3250);
   
   // 7 go left
   servo1.write(30);
   delay(1500);
   
   // go straight
   servo1.write(95);
   delay(7500);

   // 8 go left
   servo1.write(70);
   delay(500);
   
   // go straight
   servo1.write(95);
   delay(500);
   
   // 9 go right
   servo1.write(160);
   delay(500);
   
   // go straight
   servo1.write(95);
   delay(500);

   // 10 go left
   servo1.write(30);
   delay(500);
   
   // go straight
   servo1.write(95);
   delay(500);

   // 11 go right
   servo1.write(160);
   delay(500);
   
   // go straight
   servo1.write(95);
   delay(500);

   // 12 go left
   servo1.write(30);
   delay(500);
   
   // go straight
   servo1.write(95);
   delay(500);

   // victory loop
   //servo1.write(75);
   //myDCMotor01->setSpeed(200);
   //myDCMotor02->setSpeed(200);
   //delay(6000);
   
   
  
   myDCMotor01->run(RELEASE); 
   myDCMotor02->run(RELEASE);

}
// Run the motors using the run() method. The run() method takes one 
// argument: FORWARD, BACKWARD, RELEASE. RELEASE stops the motor.
// The motor command codes will execute while runSwitch is equal to 1.
void loop() 
{
 
}
 
