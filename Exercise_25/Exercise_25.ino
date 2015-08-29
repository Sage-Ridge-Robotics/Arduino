/*

Sage Ridge Robotics
Example 25

A GPS-enabled battle bot. This sketch provides an array (table) of lat and long
goals which a GPS-enabled robot will sequentially seek. The GPS code is mostly from
the Adafruit example set.

This is a pretty simple approach. Check to see if we have arrived at our final goal.
If we have, signal with the LED. If not, move forward, check the bearing of travel 
against the direction of the current goal and change direction of travel if needed.
Plenty of room for improvement, streamlining, and fancy signaling.

Adding a compass+accelerometer breakout board and code would allow the direction the
robot is facing to be easily acquired rather than using the GPS chip's direction of
travel capability which requires the bot to be moving to be accurate.

Christopher von Nagy

*/

// LOAD LIBRARIES ---------------------------------------------------------------------------------------------

// GPS class and software serial solution
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>

// Adafruit MotorShield
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

// avr math library
#include <math.h>

// GPS stuff ---------------------------------------------------------------------------------------------
// See exercise 24 for an explanation
#define GPSECHO false
SoftwareSerial mySerial(8, 7);
boolean usingInterrupt = false;
void useInterrupt(boolean);

// Variables ---------------------------------------------------------------------------------------------
const int numGoals = 6;
const int leftMotorPort  = 1; 
const int rightMotorPort = 2;
const int motorInterval = 5000;
int currentGoal = 0;
int targetAccuracy = 4;            // If we are within 4 meters of the target, move on to the next.

// Navigation goals array --------------------------------------------------------------------------------
// Targets are in decimal degrees. Latitiude is navGoal[goal][0]; Longitude navGoal[goal][1]

float navGoals[numGoals][2] =
 {
    {39.407543, -119.799252}, 
    {39.407888, -119.799872},
    {39.407983, -119.799998},
    {39.408112, -119.800130},
    {39.408384, -119.800179},
    {39.409531, -119.800293}
};


// Instantiate! ------------------------------------------------------------------------------------------
Adafruit_GPS GPS(&mySerial);
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *myLeftDCMotor = AFMS.getMotor(leftMotorPort);
Adafruit_DCMotor *myRightDCMotor = AFMS.getMotor(rightMotorPort);

// Setting things up, including the interrupt
void setup()  
{
    
  Serial.begin(115200);
  Serial.println("Start setup . . . ");
  
  Serial.println("GPS ");
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  useInterrupt(true);
  
  Serial.println("Motors ");
  AFMS.begin();
  
  Serial.println(" . . . Setup complete");
  
}

// MORE GPS stuff ----------------------------------------------------------------------------------------
SIGNAL(TIMER0_COMPA_vect) {
  char c = GPS.read();
#ifdef UDR0
  if (GPSECHO)
    if (c) UDR0 = c;  
#endif
}

void useInterrupt(boolean v) {
  if (v) {
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
    usingInterrupt = true;
  } else {
    TIMSK0 &= ~_BV(OCIE0A);
    usingInterrupt = false;
  }
}

uint32_t timer = millis();

//The main loop -----------------------------------------------------------------------------------------
void loop(){
  
  // Really, just use the interrupt apprach
  if (! usingInterrupt) {
    char c = GPS.read();
    if (GPSECHO)
      if (c) Serial.print(c);
  }
  
  // If a new NMEA data sentence is not available. Try again.
  if (GPS.newNMEAreceived()) {
    if (!GPS.parse(GPS.lastNMEA()))
      return;
  }

  // if millis() or timer wraps around, we'll just reset it
  if (timer > millis())  timer = millis();
  
  // What is our current target?
  float t_latitude = navGoals[currentGoal][0];
  float t_longitude = navGoals[currentGoal][1];

  // approximately every 4 seconds or so, check the location and adjust movement vector
  if (millis() - timer > 5000) { 
    timer = millis(); // reset the timer
    if (GPS.fix) {    
      
       Serial.print("Satellites: "); Serial.println(GPS.satellites);

      //Query what we need to know
      float c_latitude = GPS.latitudeDegrees;
      float c_longitude = GPS.longitudeDegrees;
      float altitude = GPS.altitude;
      float botSpeed = GPS.speed/0.514444444;
      float compassBearing = GPS.angle;
      
       // Calculate distance to target with Haversine formula and the forward azimuth
       // Correct direction of travel if necessary       
       float tr_latitude = radians(t_latitude);  
       float cr_latitude = radians(c_latitude);
       float tr_longitude = radians(t_longitude);
       float cr_longitude = radians(c_longitude);
       float delta_latitude = radians(t_latitude - c_latitude);
       float delta_longitude = radians(t_longitude - c_longitude);
       long  R = 6371000; // Mean radius of earth at equator in meters
       
       // Start driving if we are not at the final target location, otherwise signal our arrival
       if (currentGoal > (numGoals - 1)) {Serial.println("We are there!"); digitalWrite(13,HIGH); return;} 
       else {driveForward();}
     
       // Distance
       float a = pow(sin(delta_latitude/2),2) + cos(c_latitude) * cos(t_latitude) * pow(sin(delta_longitude/2),2);
       float c = 2 * atan2(sqrt(a), sqrt(1-a));
       float t_distance = R * c;
     
       // Forward azimuth
       float x = sin(delta_longitude) * cos(tr_latitude);
       float y = cos(c_latitude) * sin(t_latitude) - sin(c_latitude) * cos(t_latitude) * cos(delta_longitude);
       float f_azimuth = degrees(atan2(x,y));
       if (f_azimuth < 0) {f_azimuth = 360 + f_azimuth;}
       
       float azimuthCorrection = f_azimuth - compassBearing;
       Serial.print("Current location: "); Serial.print(c_latitude); Serial.print(" latitude "); Serial.print(c_longitude); Serial.println(" longitude "); 
       Serial.print("Current distance from target "); Serial.print(currentGoal); Serial.print(": "); Serial.println(t_distance);
       Serial.print("Current bearing: "); Serial.println(compassBearing);
       Serial.print("Forward azimuth: "); Serial.println(f_azimuth);
       Serial.print("Azimuth correction: "); Serial.println(azimuthCorrection);
       
       if (azimuthCorrection > 0 || azimuthCorrection < 0){ rotate_to_f_azimuth(azimuthCorrection); }
       
       // Check if we are at the current goal. If so, increment the goal.
       // targetAccuaracy should work 
       if (t_distance <= targetAccuracy) { currentGoal++; }
      
    }
    else {full_stop();}
  }
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
