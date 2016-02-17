/*

Sage Ridge Robotics
Example 8
Controlling an LED with a potentiometer using PWM or
Pulse Wave Modulation. 

We use the analogWrite() function to send a digital
square wave out of the Arduino to approximate an analog
voltage value. analogWrite actually turns the pin on and
off a a high rate to generate the square wave.

*/

// potPin is set to analog pin A2 on the Arduino board.
// ledPin is set to digital pin 3 (pins 5,6, and 9 are
// also allowed). potVal is also declared and will hold values read 
// from analog pin A2.
int potPin = 2;
int potVal;
int ledPin = 3;
 
// Set the motorPin to output to write the PWM signal. 
void setup() 
{ 
  pinMode(ledPin, OUTPUT);   // sets the pin as output
} 


// The main loop reads the value of the potentiometer
// then scales it appropriately for analogWrite() with map(). 
// analogWrite() outputs values between 0 and 255.
void loop() 
{ 
  potVal = analogRead(potPin);
  potVal = map(potVal, 0, 1023, 0, 255);     
  analogWrite(ledPin, potVal);                  
}


