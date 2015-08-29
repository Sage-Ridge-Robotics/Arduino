/*

Sage Ridge Robotics
Example 20

Sonar sensor. This sketch reports distance values from 0 - 6.45 meters 
over a serial connection. There is a companion Processing sketch to 
graphically depict your data.

When a 5v power supply is used with the sensor, the analog voltage output of
the device is aprox. 3.86 mV/cm.

This sketch requires the following: 1) sensor vin connected to 5v; 2) sensor
ground connected to common ground; 3) sensor RX pin connected to digital 13;
4) sensor pin AN connected to analog pin 0 (A0) on the Arduino.

This sketch is modified from the example sketch published by Adafruit, LLC 
on github. This code is in the public domain.

*/

// Declare variables used in this sketch. Distance expressed as voltage 
// is connected to analog pin A0. Digital pin 13 is used to turn the
// device on and off.
const int pingPin = 13;
const int voltagePin = 0;
int voltage = 0;
float distance = 0;

// Open a serial connection.
void setup() {

  pinMode(pingPin, OUTPUT);
  Serial.begin(9600);
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
  
}

