/*

Sage Ridge Robotics
Exercise 10
Controlling DC motors with the Arduino Motor Shield

The amount of current available from the Arduino board is limited
and is not appropriate for larger applications like robotics. In
order to power larger inductive loads, we will use the Arduino
Motor shield which is capable of driving heavier loads. The Motor
Shield can drive a variety of inductive loads, such as selonoids,
DC motors, and stepper motors.

The shield has a maximum rating of 1.2A per channel 
and 3A peak current capability. We will power motors from a separate 
power supply than the Arduino. It is generally an extraordinarily
bad idea to use the same power supply.

The shield can drive two steppers or four DC motors. Speed is
eight bit: 00000000.

To use the shield, we need the library from 
https://github.com/adafruit/Adafruit_Motor_Shield_V2_Library

Code adapted from Adafruit Industries, LLC.
This code is in the public domain.

*/

// Load MotorShield libraries. These must be previously installed in your
// personal Arduino/Libraries folder.
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
 
// Declare variables used globally in the Sketch.  
// An Adafruit MotorShield has four motor ports, 1 -- 4.
// dWait and dRUN are used in delay() in the main loop of the program.
int motorPort01 = 1;
int dWait = 1000;
int dRUN = 4000;

// Instantiate an instance (object) of the Adafruit_MotorShield class 
// to control a physical servo motor. The constructor method takes
// an option argument specifying the I2C communication address of 
// the shield. The default is 0x60. Our Adafruit_MotorShield object
// is called AFMS here. 
Adafruit_MotorShield AFMS = Adafruit_MotorShield();  

// Instantiate an instance of the DC Motor class
Adafruit_DCMotor *myDCMotor01 = AFMS.getMotor(motorPort01);
 
 
// Call the begin() method of the MotorShield object to
// ready the object for commands and set the default speed
// of the motor. Speed ranges from 0 to 255. The AFMS object
// begin() method takes an option argument that spcifies the
// maximum frequency. The default is 16 KHz.
void setup() 
{ 
  AFMS.begin();
  myDCMotor01->setSpeed(100);
} 

// Run the motor using the run() method. The run() method takes one 
// argument: FORWARD, BACKWARD, RELEASE. RELEASE stops the motor.
void loop() 
{
  myDCMotor01->run(FORWARD);
  delay(dRUN);
  myDCMotor01->run(RELEASE);
  delay(dWait);
  myDCMotor01->run(BACKWARD);
  delay(dRUN);
  myDCMotor01->run(RELEASE);  
}




