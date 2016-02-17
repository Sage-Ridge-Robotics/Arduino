/*

Sage Ridge Robotics
Example 26

Compass BattleBot (incomplete)

*/

// LOAD LIBRARIES ---------------------------------------------------------------------------------------------

// Compass and accelerometer 
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>

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
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);  // Compass subsystem
Adafruit_MotorShield AFMS = Adafruit_MotorShield();                    // Drive subsystem
Adafruit_DCMotor *myLeftDCMotor = AFMS.getMotor(leftMotorPort);
Adafruit_DCMotor *myRightDCMotor = AFMS.getMotor(rightMotorPort);

// Setting things up, including the interrupt
void setup()  
{
    
  Serial.begin(115200);
  Serial.println("Start setup . . . ");
  
  if(!mag.begin())
  {
    Serial.println("LSM303 compass system not detected ... Check your wiring!");
    while(1);
  }
  mag.enableAutoRange(true);
  displaySensorDetails();
  
  Serial.println("Motors ");
  AFMS.begin();
  
  Serial.println(" . . . Setup complete");
  
}


//The main loop -----------------------------------------------------------------------------------------
void loop(){
  
  // Get new sensor event from compass+accelerometer chip
  sensors_event_t event; 
  mag.getEvent(&event);
  
      float compassBearing = (atan2(event.magnetic.y,event.magnetic.x) * 180) / PI;
            
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

void displaySensorDetails(void)
{
  sensor_t sensor;
  mag.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" uT");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" uT");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" uT");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}
