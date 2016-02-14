/*

Sage Ridge Robotics
Exercise 12

Controlling DC motors and servos with an array of control codes. This
code can be used to control Ada's Challenge robots.

The motor control array uses the following code:

  XXX,L,YYY,R,ZZZ
  
  Where,
  XXX  Angle of servo, for instance 020, 090, 170
  L    Direction of motor01, 1 [forward], -1 [reverse], 0 [release]
  YYY  Speed of motor01, 000, 010, 250.
  R    Direction of motor02
  ZZZ  Direction of motor02
  
This sketch could be modified to read and execute motor commands 
from a file on an SD card.
  
*/

// Load MotorShield libraries and servo.h library.  
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

// Declare variables used globally in the Sketch.  
// Define constant (unchanging) minimum and maximum servo positions.
// Declare an array, directions, to hold instruction sets.
int motorPort01 = 1;               // Set first motor pin. This sketch assumes 
                                   // motorPort01 is the left drive motor.
int motorPort02 = 4;               // Set second motor pin
int servoPos = 90;                 // Sets the initial servo angle to 90 
                                   // degrees.

const int motorInterval = 4000;    // Sets length of time a motor command
                                   // runs. 4000 ms is four seconds. This value
                                   // will require experimentation. Use motor speed
                                   // to control distance rather than time. This may
                                   // not be an optimal solution for racing.

const int numCommands = 6;         // The number of motor commands given below. This must be
                                   // declared as a "read-only" constant value.

// We initialize the commands matrix, a two-dimensional matrix.
// As above, each command consists of 5 integer numbers that control the servo
// position for steering, the direction of rotation of two main motors, and their
// speed of rotation. 
int directions[numCommands][5] =
 {
    {90,1,50,1,50},
    {450,0,0,-1,10},
    {90,1,25,1,25},
    {135,-1,25,-1,25},
    {90,-1,100,-1,100},
    {90,0,0,0,0}
  };
  
// Instantiate objects of the servo, Adafruit_MotorShield,
// and Adafruit_DCMotor classes.
Servo myServo0;   
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *myDCMotor01 = AFMS.getMotor(motorPort01);
Adafruit_DCMotor *myDCMotor02 = AFMS.getMotor(motorPort02);

// Open the serial device and attach the servo object, 
// myServo0, to digital pin 9. Set the default 
// speed of the two DC motors.
void setup() 
{ 
  Serial.begin(9600);
  myServo0.attach(9);
  AFMS.begin();
  myDCMotor01->setSpeed(10);
  myDCMotor02->setSpeed(10);
} 

// The servo and motor commands below use the values stored in the directions array initialized above. The
// sketch sets the direction of both motors and the servo first, then the speed of both. At the end,
// the sketch calls for a full stop of all the motors and resets the servo to 90 (zero degrees).
// Arrays are zero indexed, so we start reading the array with zeros, the first position. 
void loop() 
{
  for (int i = 0; i < numCommands; i++){
    
    myServo0.write(directions[i][0]);
    
    if (directions[i][1] == 1){myDCMotor01->run(FORWARD);} 
    else if (directions[i][1] == -1){myDCMotor01->run(BACKWARD);}
    else {myDCMotor01->run(RELEASE);}
 
    if (directions[i][3] == 1){myDCMotor02->run(FORWARD);} 
    else if (directions[i][3] == -1){myDCMotor02->run(BACKWARD);}
    else {myDCMotor02->run(RELEASE);}

    myDCMotor01->setSpeed(directions[i][2]);
    myDCMotor02->setSpeed(directions[i][4]);
   
    delay(motorInterval); 
  
  }
  
  myDCMotor01->setSpeed(0);
  myDCMotor02->setSpeed(0); 
  myDCMotor01->run(RELEASE);
  myDCMotor02->run(RELEASE);
  myServo0.write(servoPos);
  
}

