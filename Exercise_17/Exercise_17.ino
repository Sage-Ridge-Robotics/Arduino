/*

Sage Ridge Robotics
Example 17

***VERY IMPORTANT: This sensor uses 3.3v! Do not use 5v to power the sensor!***

Lux sensor. This sketch demonstrates how to read data arriving
from the AdaFruit, LLC. TSL2561 lux sensor. We will send values
over a serial link to your computer. 

The device returns information on the intensity of human-visible light, 
infra-red light, and full-spectrum (visible and infrared) light. Data is 
returned in Lux, a measure of reflected light intensity. (Lumens are the SI
unit for emitted light, like that of the sun or a light bulb.)

The sensor uses the I2C communication protocol and can be set to one of
three addresses so you can have up to two other devices on the I2C
bus with the lux sensor.

In this sketch, lux values are reported over a serial connection. 

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
  Serial.println("Light Sensor Test"); Serial.println("");
  
  /* Initialise the sensor */
  if(!tsl.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.print("Ooops, no TSL2561 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  
  /* Display some basic information on this sensor */
  displaySensorDetails();
  
  /* Setup the sensor gain and integration time */
  configureSensor();
  
  /* We're ready to go! */
  Serial.println("");

}

void loop() { 
  /* Get a new sensor event */ 
  sensors_event_t event;
  uint16_t combined, ir_only;
  
  tsl.getEvent(&event);
  tsl.getLuminosity(&combined, &ir_only);
 
  /* Display the results (light is measured in lux) */
  if (event.light)
  {
    Serial.print(event.light); Serial.println(" lux");
    Serial.print(combined); Serial.println(" lux (combined)");
    Serial.print(ir_only); Serial.println(" lux (ir)");
  }
  else
  {
    /* If event.light = 0 lux the sensor is probably saturated
       and no reliable data could be generated! */
    Serial.println("Sensor overload");
  }
  delay(250);
}

// Two functions are defined and used in this sketch.


// This function sends information about the sensor over a serial link
// to your computer. The function is called without parameters.
void displaySensorDetails(void)
{
  sensor_t sensor;
  tsl.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" lux");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" lux");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" lux");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

// This function configures the sensor gain and integration time. The default gain
// setting is auto allowing the board to switch itself between daylight and lowlight
// modes. The default integration time is 12 milliseconds.
void configureSensor(void)
{
  /* You can also manually set the gain or enable auto-gain support */
  // tsl.setGain(TSL2561_GAIN_1X);      /* No gain ... use in bright light to avoid sensor saturation */
  // tsl.setGain(TSL2561_GAIN_16X);     /* 16x gain ... use in low light to boost sensitivity */
  tsl.enableAutoRange(true);            /* Auto-gain ... switches automatically between 1x and 16x */
  
  /* Changing the integration time gives you better sensor resolution (402ms = 16-bit data) */
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);      /* fast but low resolution */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);  /* medium resolution and speed   */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);  /* 16-bit data but slowest conversions */

  /* Update these values depending on what you've set above! */  
  Serial.println("------------------------------------");
  Serial.print  ("Gain:         "); Serial.println("Auto");
  Serial.print  ("Timing:       "); Serial.println("13 ms");
  Serial.println("------------------------------------");
}




