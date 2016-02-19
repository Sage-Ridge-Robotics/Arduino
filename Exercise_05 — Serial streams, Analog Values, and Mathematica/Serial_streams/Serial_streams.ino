/*

Sage Ridge Robotics
Example 05

Streaming analog input values over a serial link. This Arduino sketch
pairs with the Matehmatica notebook, "Serial streams — Exporting data from
an Arduino", and SRS Robotics Processing Exercises 1 through 5. 

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
 
// In the main loop we read the analog value of potPin every 100 milliseconds
// and stream the value out over the serial connection. The on-board DAC
// (digital to analog converter) will convert the voltage signal variance, 0
// to 5 volts, to a digital number between 0 and 1023. 
void loop() 
{
  int reading  = analogRead(potPin);
  Serial.print(reading);
  Serial.println(",\0");                     // Add an line terminator to make the Processing sketch more happy.   
  delay(100);
}

