/*

Sage Ridge Robotics
Example 19

TCS34725 color sensor. This sketch reports lux values over a serial 
connection. There is a companion Processing sketch (Example 8) to graphically 
depict your data.

This sketch is modified from the example sketch published by Adafruit, LLC 
on github. This code is in the public domain.

*/

//Load required libraries
#include <Wire.h>
#include "Adafruit_TCS34725.h"

// our RGB -> eye-recognized gamma color
byte gammatable[256];
int led01 = 13;

// Instantiate an instance of the TCS34725 class
// This allows our sketch to talk to the color sensor
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

// We open a serial connection, test for the presence of the sensor,
// configure the sensor, and retrieve basic information.
void setup() {
  
  pinMode(led01, OUTPUT);
  Serial.begin(9600);
  if (tcs.begin()) {
    for (int i = 0; i < 10; i++) {
      digitalWrite(led01, HIGH); // On-board LED flashes with success
      digitalWrite(led01, LOW);
    }
    tcs.setIntegrationTime(TCS34725_INTEGRATIONTIME_50MS);
    tcs.setGain(TCS34725_GAIN_1X);
  } else {
    digitalWrite(led01, LOW); // On-board LED dark with failure
    while (1); // halt!
  }
  
  for (int i=0; i<256; i++) {
    float x = i;
    x /= 255;
    x = pow(x, 2.5);
    x *= 255;
    gammatable[i] = x;      
  }

  

}

// With each loop we turn on the LED, take a raw color reading, calculate
// luminosity in lux and color temperature from the raw RGB values, turn
// off the LED then report the values via the serial object. If there is
// an error the sketch sends zeros.

// Reported values:

// clear is the combined white light value.
// red is the RGB red value.
// green is the RGB green value.
// blue is the RGB blue value.
// temp is the black-body radiation color temp in degrees Kelvins, 
// like furnace-hot iron and solar atmospheres.
// lux is the illuminance value in SI lux units.

void loop() {
  
  uint16_t clear, red, green, blue, temp, lux;
  tcs.setInterrupt(false); // turn on LED
  delay(60);  // takes 50ms to read 
  tcs.getRawData(&red, &green, &blue, &clear);
  temp = tcs.calculateColorTemperature(red, green, blue);
  lux = tcs.calculateLux(red, green, blue);
  tcs.setInterrupt(true);  // turn off LED
  
  // Scale RGB values. If you change this, change the colorMode scale
  // in the Processing sketch.
  uint32_t sum = clear;
  float r = red; r /= sum;
  float b = green; b /= sum;
  float g = blue; g /= sum;
  r *= 256; g *= 256; b *= 256;


  
  // Send the lux value 
  if (lux > 0)
  {
  Serial.print(lux); Serial.print(",");
  Serial.print(gammatable[(int)r]); Serial.print(",");
  Serial.print(gammatable[(int)g]); Serial.print(",");
  Serial.print(gammatable[(int)b]); Serial.print(",");
  Serial.print(temp); Serial.print("");
  Serial.println();
  }
  else
  {
    Serial.println("0,0,0,0,0 \0");
  }
  delay(250);
}


