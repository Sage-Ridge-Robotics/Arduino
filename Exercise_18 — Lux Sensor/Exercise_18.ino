/*

Sage Ridge Robotics
Example 18

***VERY IMPORTANT: This sensor uses 3.3v! Do not use 5v to power the sensor!***

Lux sensor. This sketch reports lux values over a serial connection. There is
a companion Processing sketch to graphically depict your data.

This sketch is modified from the example sketch published by Adafruit, LLC 
on github. This code is in the public domain.

*/

//Load required libraries
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h> 

// Instantiate an instance of the TSL2561 class
// This allows our sketch to talk to the lux sensor
Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);


// We open a serial connection, test for the presence of the sensor,
// configure the sensor, and retrieve basic information.
void setup() {
  
  Serial.begin(9600);
  if(!tsl.begin())
  {
    Serial.print("Ooops, no TSL2561 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  
  configureSensor();
  Serial.println("");

}

// The loop is set to use the standardized SI lux value for the sensor
// via the unified sensor library. It is also possible to get 
// combined or infrared luminisoty values for the snsor by commenting
// the instantiation of a sensors_event_t object and tsl.getEvent method out 
// and un-commenting the creation of variables to hold the
// luminosity values and getLuminosity method. If you do so, remember to 
// change which values are reported by the serial object. The sketch
// reports a value of zero if their is insufficient light for a response of
// if the sensor is overloaded with too much light. Use map() to rescale
// the values reported by the Serial object if necessary.
void loop() { 
  sensors_event_t event;
  // uint16_t combined, ir_only;
  tsl.getEvent(&event);
  //tsl.getLuminosity(&combined, &ir_only);
 
  // Send the lux value 
  if (event.light)
  // if (ir_only != null)
  {
    Serial.print(event.light); Serial.println(" \0");
    //Serial.print(ir_only); Serial.println("\0");
  }
  else
  {
    Serial.println("0 \0");
  }
  delay(250);
}

// This function configures the sensor gain and integration time. The default gain
// setting is auto allowing the board to switch itself between daylight and lowlight
// modes. The default integration time is 101 milliseconds.
void configureSensor(void)
{
  /* You can also manually set the gain or enable auto-gain support */
  // tsl.setGain(TSL2561_GAIN_1X);      /* No gain ... use in bright light to avoid sensor saturation */
  // tsl.setGain(TSL2561_GAIN_16X);     /* 16x gain ... use in low light to boost sensitivity */
  tsl.enableAutoRange(true);            /* Auto-gain ... switches automatically between 1x and 16x */
  
  /* Changing the integration time gives you better sensor resolution (402ms = 16-bit data) */
  //tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);      /* fast but low resolution */
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);  /* medium resolution and speed   */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);  /* 16-bit data but slowest conversions */
}




