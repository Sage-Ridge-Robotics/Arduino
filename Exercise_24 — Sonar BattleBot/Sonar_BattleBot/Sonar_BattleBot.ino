/*

Sage Ridge Robotics
Example 24

A sonar-enabled battle bot. This is a simple example of integrating an ultrasonic sensor into
a simple battle bot (sumo bot). In the example configuration here, the sonar obtains the distance
from the sensor to the surface during setup(). The robot will randomly move in the main loop()
using the sonar to signal a dropoff to be avoided.

Experimenting

1) Modify the position of the sonar on the robot and this sketch to create obstacle avoiding
behavior on the part of the machine.

2) Experiment with adding a second sonar and modifing the code to detect objects to attack (push out of the ring
or flip).

Copyright (c) 2016 Christopher von Nagy
This code is in the public domain.

*/

// LOAD LIBRARIES -----------------------------------------------------------------------------------------

#include <math.h>

// Adafruit MotorShield
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

// CREATE VARIABLES ---------------------------------------------------------------------------------------

// For the sonar system
const int pingPin = 13;
const int voltagePin = 0;
int voltage = 0;
int grnd_distance = 0;
int distance = 0;

// For the motors
const int leftMotorPort  = 3; 
const int rightMotorPort = 4;

// For navigation
// navPower will be randomized -255 .. 255.
const int navInterval = 2500;
int navPower0 = 0;
int navPower1 = 0;

// INSTANTIATE! ------------------------------------------------------------------------------------------
Adafruit_MotorShield AFMS = Adafruit_MotorShield();                    // Drive subsystem
Adafruit_DCMotor *l_Motor = AFMS.getMotor(leftMotorPort);
Adafruit_DCMotor *r_Motor = AFMS.getMotor(rightMotorPort);

void setup()  
{
  
  // Set up serial to report navigation settings  
  Serial.begin(9600);

  // initialize the ultrasonic senosr and run calibrate cycle
  pinMode(pingPin, OUTPUT);
  delay(250);                  // Sensor power up.
  digitalWrite(pingPin, HIGH); // Calibration cycle
  delay(100);
  digitalWrite(pingPin, LOW);  // Calibration cycle complete
  
  // Ping distance to ground
  digitalWrite(pingPin, HIGH);
  for (int i=0; i<5; i++) {
     voltage += analogRead(voltagePin);
     delay(50);      // A reading takes 49mS
   } voltage /= 5;   // We loop five times to take an average voltage.
  
  // Convert voltage to distance in cm. 
  grnd_distance = abs(round((voltage * 1.153846154) * 100));
  
  // initialize the motor shield.
  AFMS.begin();
  
  // Seed the random number generator
  randomSeed(analogRead(0));
  
}

//The main loop -----------------------------------------------------------------------------------------
void loop(){
  
  // Obtain random numbers for navPower
  navPower0 = map(random(0,198),0,198,-99,99);
  navPower1 = map(random(0,198),0,198,-99,99);
  
  // Test the waters and run away if necessary
  digitalWrite(pingPin, HIGH);
  for (int i=0; i<5; i++) {
     voltage += analogRead(voltagePin);
     delay(50); 
  } 
  voltage /= 5;
  distance = abs(round((voltage * 1.153846154) * 100));
  
  if ( distance > (grnd_distance + 5) ) {
    l_Motor->setSpeed(100);
    r_Motor->setSpeed(100);
    l_Motor->run(BACKWARD);
    l_Motor->run(BACKWARD);
    delay(3000);
  } else {
    
    // Otherwise ...
    l_Motor->setSpeed(abs(navPower0)); // Takes the absolute value of navPower0 and sets motor speed.
    r_Motor->setSpeed(abs(navPower1)); // Takes the absolute value of navPower1 and sets motor speed.
       
    if (navPower0 > 0) {l_Motor->run(FORWARD);} 
    if (navPower0 < 0) {l_Motor->run(BACKWARD);}
    if (navPower0 = 0) {l_Motor->run(RELEASE);} 

    if (navPower1 > 0) {l_Motor->run(FORWARD);} 
    if (navPower1 < 0) {l_Motor->run(BACKWARD);}
    if (navPower1 = 0) {l_Motor->run(RELEASE);}
   
    delay(navInterval);
    
  }
  
  Serial.print(navPower0);
  Serial.print(", ");
  Serial.print(navPower1);
  Serial.print(", ");
  Serial.println(voltage);

}

