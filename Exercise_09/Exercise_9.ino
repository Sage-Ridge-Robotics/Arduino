/*

Sage Ridge Robotics
Example 9
Controlling a DC motor with a potentiometer using PWM or
Pulse Wave Modulation. This will work with a small DC
Motor but not a larger motor as the current load of a larger
DC motor is too high for the Arduino's curcuitry.

We use the analogWrite() function to send a digital
square wave out of the Arduino to approximate an analog
voltage value. analogWrite actually turns the pin on and
off a a high rate to generate the square wave.

*/

// potPin is set to analog pin A0 on the Arduino board.
// motorPin is set to digital pin 3 (pins 5,6, and 9 are
// also allowed). potVal is also declared and will hold values read 
// from analog pin A0.
int potPin = 0;
int potVal;
int motorPin = 3;
 
// Set the motorPin to output to write the PWM signal. 
void setup() 
{ 
  pinMode(motorPin, OUTPUT);   // sets the pin as output
} 


// The main loop reads the value of the potentiometer
// then scales it appropriately for analogWrite() with map(). 
// analogWrite() outputs values between 0 and 255.
void loop() 
{ 
  potVal = analogRead(potPin);
  potVal = map(potVal, 0, 1023, 0, 255);     
  analogWrite(motorPin, potVal);                  
}


