/*

Sage Ridge Robotics
Example 21

Multi-sensor battle or sumo robot. 

Sensor functions.

Modify as you see fit to obtimize your robot!

Copyright (c) 2016 Christopher von Nagy
This code is in the public domain.

Speed settings may vary from 0 to 255.
Directions may be FORWARD, BACKWARD, and RELEASE (stop).

Note that motorInterval is defined in the main program file is
is the number of milliseconds a particular command executes.
Adjust as required.

*/

// calibrate_sonars() function.
boolean calibrate_sonars(void) {

  Serial.print("Calibrating sonar . . . ");

  digitalWrite(PPIN0, HIGH);
  digitalWrite(PPIN1, HIGH);
  delay(100);
  digitalWrite(PPIN0, LOW);
  digitalWrite(PPIN1, LOW);

  Serial.println(" done!");
   
  return 1;

  // Not at all necessary. Function here just to demonstrate boolean and return.  


}

// target_detect() function.
void target_detect(void) {
  
  int voltage0 = 0;
  int voltage1 = 0;
  
  int distance = 0;
  float distance0 = 0;                                  
  float distance1 = 0;
    
  // Measure distance as an average of 5 pings
  digitalWrite(PPIN0, HIGH);
  digitalWrite(PPIN1, HIGH);
  for (int i = 0; i < 5; i++){
    voltage0 += analogRead(VPIN0);
    voltage1 += analogRead(VPIN1);
    delay(50);
  }
  digitalWrite(PPIN0, LOW);
  digitalWrite(PPIN1, LOW);
  distance0 = (voltage0/5) * 115.3846154;
  distance1 = (voltage1/5) * 115.3846154;
  distance = round( (distance0 + distance1)/2 );
  
  // The algorithm here is simple. If the calculated distances are closely similar, the target is deemed ahead. If one distance
  // is greater than the other by a certain threshold, the robot turns to try to center on the target. This could
  // certainly be improved. If the averaged target distance is equal to or less than 10cm, the target is within the lunge threshold.  
  if      ( distance <= 10 ) { target_detected = 1; target_proximate = 1; target_position = 0; }
  else if ( distance > 10 && distance0 > distance1 && distance <= DIST_T )   { target_detected = 1; target_proximate = 0; target_position = -1; }
  else if ( distance > 10 && distance0 < distance1 && distance <= DIST_T  )  { target_detected = 1; target_proximate = 0; target_position =  1; }
  else if ( distance > 10 && distance0 == distance1 && distance <= DIST_T  ) { target_detected = 1; target_proximate = 0; target_position =  0; }
  else    { target_detected = 0; target_proximate == 0; target_position =  0; }
      
}

// Start lux sensor
boolean start_lux_sensor(void) {
  
  Serial.print("Starting TSL2561 . . . ");
  
  boolean sensor_status = 0;
  
  if (tsl.begin()) { 
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
    sensor_status = 1;
  } else
  {
    Serial.println("Ooops, no TSL2561 detected");
    delay(500);
  }
  return sensor_status;
  
}

// This function configures the TSL2561 sensor gain and integration time.
void configure_lux_sensor(void)
{

  // tsl.setGain(TSL2561_GAIN_1X);      /* No gain ... use in bright light to avoid sensor saturation */
  // tsl.setGain(TSL2561_GAIN_16X);     /* 16x gain ... use in low light to boost sensitivity */
  tsl.enableAutoRange(true);            /* Auto-gain ... switches automatically between 1x and 16x */
  
  /* Changing the integration time gives you better sensor resolution (402ms = 16-bit data) */
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);      /* fast but low resolution */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);  /* medium resolution and speed   */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);  /* 16-bit data but slowest conversions */

}


// Determine play surface lux value.
int detect_play_surface_lux(void) {
  
  Serial.print("Detecting averaged play surface lux value ... ");
  
  float lux;
  sensors_event_t event;
  tsl.getEvent(&event);

  if (event.light) {
    for (int i = 0; i < 5; i++) { lux += event.light; delay(250); }
    lux = lux /= 5;
    Serial.print(lux);
    Serial.println(" lux");
  } else {Serial.println("sensor error. Full stop."); while (1);}

  return round(lux);
  
}

// Detect ring boundary. Returns 1 if the boandary is detected.
boolean detect_boundary(void) {
 
  Serial.print("Detecting boundary ... ");
  
  float lux;
  boolean boundary = 0;
  
  sensors_event_t event;
  tsl.getEvent(&event);

  if (event.light) {
    for (int i = 0; i < 5; i++) { lux += event.light; delay(250); }
    lux = lux /= 5;
  } else {Serial.println("sensor error. Full stop."); while (1);}
   
  if ( lux >= lux_threshold ) { boundary = 1; Serial.println("present"); } else { Serial.println("absent"); }
     
   return boundary;
  
}







