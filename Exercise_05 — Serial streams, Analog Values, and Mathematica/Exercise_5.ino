/*
Sage Ridge Robotics
Example 05
Streaming analog input values over a serial link

Code adapted from Adafruit, LLC
This code is in the Public Domain

*/
 
// Here we set the value of the variable potPin to an interger value, 0
int potPin = 0;

// In setup() we open the serial output for streaming 
// at a rate of 9600 characters per second
// We open the serial port for streaming by instantiating a serial object
// of the serial class.
void setup() 
{
  Serial.begin(9600);
}
 
// In the main loop we read the analog value of potPin every 500 milliseconds
// and stream the value out over the serial connection.
void loop() 
{
  int reading  = analogRead(potPin);
  Serial.println(reading);
  delay(100);
}

