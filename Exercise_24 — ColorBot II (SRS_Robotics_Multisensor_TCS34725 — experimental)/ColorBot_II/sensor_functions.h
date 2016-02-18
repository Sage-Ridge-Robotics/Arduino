/*

Sage Ridge Robotics
Example 24 

Sensor function library



*/

// Initialize Adafruit TSL2561 lux (light level) sensor
void init_lux_sensor(Adafruit_TSL2561_Unified *sensor, int led){
  
    if (sensor->begin()) {             
      
      sensor->setGain(TSL2561_GAIN_1X);     /* No gain ... use in bright light to avoid sensor saturation */
      // sensor->setGain(TSL2561_GAIN_16X);  /* 16x gain ... use in low light to boost sensitivity */
      // sensor->enableAutoRange(true);      /* Auto-gain ... switches automatically between 1x and 16x */
  
      // Changing the integration time gives you better sensor resolution (402ms = 16-bit data)
      sensor->setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);      // fast but low resolution
      // sensor->setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);   // medium resolution and speed
      // sensor->setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);   //16-bit data but slowest conversions

        for (int i = 0; i < 5; i++) {  // On-board LED flashes with success
          digitalWrite(led, HIGH); 
          delay(100);
          digitalWrite(led, LOW);
          delay(100);
        }
        for (int i = 0; i < 5; i++) {
          digitalWrite(led, HIGH);  
          delay(25);
          digitalWrite(led, LOW);
          delay(25);
        }
    }
    else {
          digitalWrite(led, HIGH);  // On-board LED flashes long with failure  
          delay(1000);
          digitalWrite(led, LOW);
          while (1);                  // halt! 
    }
    delay(1000);                      // let sensor "warm up"
}

// Initialize Adafruit TCS34725 color sensor
void init_color_sensor(TCS34725 *sensor, int sdaPin, int sclPin, int led) {
  
    // Expects a pointer to a TCS34725 sensor object
    // Integration times can range from 2_4MS to 700MS 
    // Gain gain can range from 1X to 60X
    *sensor = TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X, sdaPin, sclPin);

    if (sensor->begin()) {           
        sensor->enable();
        for (int i = 0; i < 5; i++) {  // On-board LED flashes with success
          digitalWrite(led, HIGH); 
          delay(100);
          digitalWrite(led, LOW);
          delay(100);
        }
        for (int i = 0; i < 5; i++) {
          digitalWrite(led, HIGH);  
          delay(25);
          digitalWrite(led, LOW);
          delay(25);
        }
    }
    else {
          digitalWrite(led, HIGH);  // On-board LED flashes long with failure
          delay(1000);
          digitalWrite(led, LOW);
          while (1);                  // halt! 
    }
    delay(1000);                      // let sensor "warm up"   
}

// Check ambient light level with the TCS34725
int check_light_level(Adafruit_TSL2561_Unified *sensor, int lt) {

    int need_headlights;
    sensors_event_t event;
    sensor->getEvent(&event);
    //tsl.getLuminosity(&combined, &ir_only);
    if (event.light <= lt){need_headlights = 1;} else {need_headlights = 0;}
    return need_headlights;

}

// Turn the headlights on or off
void switch_headlight(int h, int hl01, int hl02) {
  
  if (h) {
    digitalWrite(hl01, HIGH);
    digitalWrite(hl02, HIGH);
  } else {
    digitalWrite(hl01, LOW);
    digitalWrite(hl02, LOW);
  }

}


// Function to compare sensed color value against the target value
// deltaE = sqrt(deltaL^2 + deltaA^2 + deltaB^2)
// deltaE requires the perceptual CIE L*A*B* color system, so we have to convert. 
// Colors with small differences in deltaE are similar. CIE Lab color matching is the 
// preferred algorithm and is used by all important commerical color matching systems 
// (Apple, Adobe, Microsoft, etc), as well as open source projects.

// check_color_status works by passing a pointer to the sensor object and the LAB values of the
// target color. We use sensor as the object name within the function.
// Parameters are a pointer to the sensor object, int L, a, b values, and deltaE threshhold
int check_color_status(TCS34725 *sensor, float *L_target, float *a_target, float *b_target, float *deltaE_threshold) {
  
  int match;
  float L_sample, a_sample, b_sample, deltaL, deltaA, deltaB, deltaE;

  sensor->getLAB( &L_sample, &a_sample, &b_sample );
  deltaL = L_sample - L_target;
  deltaA = a_sample - a_target;
  deltaB = b_sample - b_target;
  deltaE = sqrt(pow(deltaLab[0],2) + pow(deltaLab[1],2) + pow(deltaLab[2],2));
  if (deltaE <= deltaE_threshold) {match = 1;} else {match = 0;}
  return match;

}


// Read sonar function. Returns 1 if the sonar detects an object 
int sonar(int critical_distance){

    int voltage = 0;
    float distance;
    int fstop = 0;
    
    pinmode(pingpin,OUTPUT);
    pinmode(vpin,INPUT);
    
    // Calibration
    digitalWrite(pingpin, HIGH);
    delay(100);
    digitalWrite(pingpin, LOW);
    
    // Measure distance as an average of 5 pings
    digitalWrite(pingpin, HIGH);
    for (int i = 0; i < 5; i++){
      voltage += analogRead(vpin);
      delay(50);
    }
    distance = (voltage/5) * 1.153846154;
    if (distance <= critical_distance) fstop = 1;
    return fstop;
}

