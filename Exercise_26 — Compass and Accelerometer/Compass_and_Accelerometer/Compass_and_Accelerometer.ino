/*

Sage Ridge Robotics
Example 26

Compass and Accelerometer. This sketch reports compass and accelerometer data to the serial stream. Experiment
with determining variations in the earth' magnetic field or in detecting and visualizing the magnetic field of 
a neodimium or other magnet placed in range od the sensor.

This sketch uses the Adafruit library.

Do not place the sensor on a magnet.

*/

// VARIABLES --------------------------------------------------------------------------------------------------
float declination = 0.2377; // radians in Reno, NV. If you are somewhere else, you will need to find your correct declination

// LOAD LIBRARIES ---------------------------------------------------------------------------------------------

// Wire library for I2C
#include <Wire.h>

// Compass and accelerometer 
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>

// avr math library
#include <math.h>

// Instantiate! ------------------------------------------------------------------------------------------
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);  // Compass subsystem

// Setting things up, including the interrupt
void setup()  
{
    
  Serial.begin(9600);
  Serial.println("Start setup . . . ");
  
  if(!mag.begin())
  {
    Serial.println("LSM303 compass and accelerometer system not detected ... Check your wiring!");
    while(1);
  }
  
  // Manually set gain and magnetometer update rate
  mag.setMagGain(LSM303_MAGGAIN_1_3);
  mag.setMagRate(LSM303_MAGRATE_3_0);
  
  /*  Magnetometer gain rates in Gauss. 
      +/- 1.3 is -1.3 -- 1.3 Gauss. Raw magnetic measurement is in gauss rather than tesla units.
      Select the smallest range encompassing your measurements for the highest precision.
      
      LSM303_MAGGAIN_1_3                        = 0x20,  // +/- 1.3
      LSM303_MAGGAIN_1_9                        = 0x40,  // +/- 1.9
      LSM303_MAGGAIN_2_5                        = 0x60,  // +/- 2.5
      LSM303_MAGGAIN_4_0                        = 0x80,  // +/- 4.0
      LSM303_MAGGAIN_4_7                        = 0xA0,  // +/- 4.7
      LSM303_MAGGAIN_5_6                        = 0xC0,  // +/- 5.6
      LSM303_MAGGAIN_8_1                        = 0xE0   // +/- 8.1
      
  */
  
  /*  Magnetometer update rates
      LSM303_MAGRATE_0_7                        = 0x00,  // 0.75 Hz
      LSM303_MAGRATE_1_5                        = 0x01,  // 1.5 Hz
      LSM303_MAGRATE_3_0                        = 0x62,  // 3.0 Hz
      LSM303_MAGRATE_7_5                        = 0x03,  // 7.5 Hz
      LSM303_MAGRATE_15                         = 0x04,  // 15 Hz
      LSM303_MAGRATE_30                         = 0x05,  // 30 Hz
      LSM303_MAGRATE_75                         = 0x06,  // 75 Hz
      LSM303_MAGRATE_220                        = 0x07   // 200 Hz
  */
  
  // Enables magnetometer autoranging
  // mag.enableAutoRange(true);
  

  // Calls function to report information on chip
  displaySensorDetails();
  
  
}


//The main loop -----------------------------------------------------------------------------------------
void loop(){
  
  // Get new sensor event from compass+accelerometer chip
  sensors_event_t event; 
  mag.getEvent(&event);
  
  // Calculate compass bearing and convert to degrees from radians. We are going to use a bit
  // of trigonometry here. Declination is defined above. The chip should be flat.
  
  // First we calculate the heading using the atan2 function, and the magnetic x and y vectors.
  float heading = atan2(event.magnetic.y,event.magnetic.x);
  heading += declination;   // heading -= declination if the declination is negative.
 
  // Convert to degrees from radians 
  if( heading <0 ) { heading += 2*PI; }
  if( heading > 2*PI ) { heading -= 2*PI; }
  float compassBearing = ( heading * 180) / PI;
  

  // Magnetic vector values are in micro-Tesla (uT)). One micro-Tesla is 0.01 G (gauss); 1 T is 10,000 G (gauss).
  Serial.print("Bearing: "); Serial.print(compassBearing); Serial.print("  ");
  Serial.print("X: "); Serial.print(event.magnetic.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.magnetic.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.magnetic.z); Serial.print("  ");
  Serial.println("uT");

  // Acceleration is in m/sˆ2.
  Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  ");
  Serial.println("m/s^2 ");
  delay(500);

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
