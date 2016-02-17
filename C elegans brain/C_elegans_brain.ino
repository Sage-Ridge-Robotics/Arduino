/*

Sage Ridge Robotics
C. elegans brain
Christopher von Nagy (2015)
cvonnagy@sageridge.org

This code implements an Arduino version of the C. elegans connectome. It is derived from Timothy Busbice (2014) SimpleConnectomev3 Python 
implementation. Similar to Tim's code, this version of the connectome does not set up individual neurons connected via UDP. Other
implementations run the brain as a cluster of 300 programs, each a neuron, communicating via UDP. That is not possible on the Arduino.

Like the Python implementation, this code employs arrays to represent the neurons, their interconnectivity, and their 
excitation state.

Like Tim's version, this implementation of the C. elegans brain on silicon uses a sonar sensor as the worm's nose and touch sensors for left and right-side sensory neurons. A lux sensor is used to detect food. The The sketch is implemented to run on a mini-sumo robot with an Adafruit motor shield controlling two motors to manage forward, backward, and turning motions.

*/

// Load MotorShield libraries. These must be previously installed in your
// personal Arduino/Libraries folder. 
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h> 
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"


// declare data structures for C. elegans connectome
struct connectome_node {
  string sn;
  string dn;
  int b;
};

connectome_node connectome[3953]
 
// Declare variables used globally in the Sketch.  
// dWait and dRUN are used in delay() in the main loop of the program.
int motorPort01 = 1;
int motorPort02 = 2;

int dWait = 1000;
int dRUN = 4000;

int runSwitch = 1;

// Instantiate an object of the Adafruit_MotorShield class 
// to control a physical servo motor. 
Adafruit_MotorShield AFMS = Adafruit_MotorShield();  

// Instantiate objects of the DC Motor class
Adafruit_DCMotor *myDCMotor01 = AFMS.getMotor(motorPort01);
Adafruit_DCMotor *myDCMotor02 = AFMS.getMotor(motorPort02);
 
 

// Declare variables used in this sketch. Distance expressed as voltage 
// is connected to analog pin A0. Digital pin 13 is used to turn the
// device on and off.
const int pingPin = 13;
const int voltagePin = 0;
int voltage = 0;
float distance = 0;



// Open a serial connection.
void setup() {
  
  AFMS.begin();
  myDCMotor01->setSpeed(0);
  myDCMotor02->setSpeed(0);  
  
  Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);
  if(!tsl.begin())
  {
    Serial.print("Ooops, no TSL2561 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  configureSensor();




  pinMode(pingPin, OUTPUT);
  delay(250); // Sensor power up.
  digitalWrite(pingPin, HIGH); // Calibration cycle
  delay(100);
  digitalWrite(pingPin, LOW); // Calibration cycle complete
  
}

// The range finder is activated once per loop. The resulting 
// voltage is converted to meters and distance is sent over the serial
// connection.

void loop() { 

  digitalWrite(pingPin, HIGH);
  for (int i=0; i<5; i++) {
     voltage += analogRead(voltagePin);
     delay(50);      // A reading takes 49mS
   } voltage /= 5;  // We loop five times to take an average voltage.
  
  // Convert voltage to distance in cm. The value used here is calibrated but
  // probably needs adjustment. map() can also be used as long as the highest
  // output form the AN pin is determined. 
  distance = voltage * 1.153846154; 
  // distance = map(voltage,0,600,0,980);
  digitalWrite(pingPin, LOW);
  Serial.print(distance); Serial.println();
  
      myDCMotor01->run(FORWARD);
    myDCMotor02->run(FORWARD);  
    delay(dRUN);
    myDCMotor01->run(RELEASE);
    myDCMotor02->run(RELEASE);
    delay(dWait);
    myDCMotor01->run(BACKWARD);
    myDCMotor02->run(BACKWARD);
    
    
    
    
    
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

void create_connectome(void)
{
  
  
  
}



