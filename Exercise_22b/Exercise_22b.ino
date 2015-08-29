/*

Sage Ridge Robotics
Color Bot morning. This example sketch integrates a luminosity, ultrasonic distance, 
and two (2) color sensors to drive a single steering servo and two motors allowing a robot 
to follow a color-marked course.

This sketch provides the core logic of the robot and depends on the color sensing subsystem.

Stuart Hayes (w. corrections by C v Nagy)

*/

// LOAD LIBRARIES ---------------------------------------------------------------------------------------------

// Fundamental libraries
#include <Wire.h>

// Modified SRS TCS34725 library
#include <SRS_Robotics_Adafruit_TCS34725.h>

// Target color in *LAB
const float L_target = 102.81;                              
const float a_target = 23.93;
const float b_target = 29.92;

// DeltaE is a statistic that allows a numeric comparison of 
// perceptual (how you see it) color similarity
const int deltaE_threshold = 1;

// Arduino pin specifications
const int status_LED = 13;          // Used to indicate status         
const int opin = 12;                // Boolean output of color match state
const int spin = 11;                // Boolean output of color match system status
int color_status = 0;               // Initial color status

// Instantiate objects
// Gain and integration time can be adjusted
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X); 

void setup() {
  
     // Set pin modes and initialize output to LOW (false)
     pinMode(status_LED, OUTPUT);
     pinMode(opin, OUTPUT);
     pinMode(spin, OUTPUT);
     
     digitalWrite(spin, LOW);
     digitalWrite(opin, LOW);
     
     // Initialize serial system for error reporting
     Serial.begin(9600);
    
     // Initialize color detection system
     if (tcs.begin()) {
        digitalWrite(spin, HIGH);
        for(int i = 0; i < 5; i++){digitalWrite(status_LED, HIGH); delay(50); digitalWrite(status_LED, LOW);}   
        tcs.disable();
      }
      else {
       digitalWrite(spin, LOW);
       digitalWrite(status_LED, HIGH); 
       while(1); // halt!
      }
}

void loop(){
  
     if(check_color_status(&tcs, L_target, a_target, b_target, deltaE_threshold)) {digitalWrite(opin, HIGH);} else {digitalWrite(opin,LOW);}
     
}
 
int check_color_status(Adafruit_TCS34725 *sensor,float L_target, float a_target, float b_target, float deltaE_threshold) {
      sensor->enable();
      int match;
      float L_sample, a_sample, b_sample, deltaL, deltaA, deltaB, deltaE;
      tcs.setInterrupt(false);
    
      sensor->getLAB( &L_sample, &a_sample, &b_sample );
      deltaL = L_sample - L_target;
      deltaA = a_sample - a_target;
      deltaB = b_sample - b_target;
      deltaE = sqrt(pow(deltaL,2) + pow(deltaA,2) + pow(deltaB,2));
      if (deltaE <= deltaE_threshold) {match = 1;} else {match = 0;}
      Serial.println(L_sample);
      Serial.println(a_sample);
      Serial.println(b_sample);
      Serial.println(deltaE);
      return match;
      tcs.setInterrupt(true);
      sensor->disable();
}

