/*

Sage Ridge Robotics
Example 13

Controlling a servo with a Mathematica dashboard. 

Code adapted from Arduino.cc and Adafruit Industries, LLC.
This code is in the public domain.

*/

// Load MotorShield libraries and servo.h library.  
#include <Servo.h>

// Declare variables used globally in the Sketch.  
// Define constant (unchanging) minimum and maximum servo positions
int servoPos = 90;  

const int servoMin = 10;
const int servoMax = 170;
 
// Instatiate objects of the servo class.
Servo myServo0;   

// Open the serial device and attach the servo object, 
// myServo0, to digital pin 9. This is where we 
// attached the physical servo' control wire (yellow).
void setup() 
{ 
  Serial.begin(9600);
  myServo0.attach(9);
} 

// Move servo to position sent over serial link by parsing an integer value
// from the serial stream, checking against servoMin and servoMax
// to protect the servo, and passing the legal value to the write method of
// the servo object.
void loop() 
{
  if (Serial.available() > 0) {
    servoPos = Serial.parseInt();
    constrain(servoPos, servoMin, servoMax);
    myServo0.write(servoPos);
  }
}


