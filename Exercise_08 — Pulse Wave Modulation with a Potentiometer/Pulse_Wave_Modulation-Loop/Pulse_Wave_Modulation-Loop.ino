/*

Sage Ridge Robotics
Pulse Wave Modulation.

This script will step through PWM output values allowing PWM
to be demonstrated on an oscilloscope. Adjust the delay() as
appropriate.

*/

// potPin is set to analog pin A2 on the Arduino board.
// ledPin is set to digital pin 3 (pins 5,6, and 9 are
// also allowed). potVal is also declared and will hold values read 
// from analog pin A2.
int pwmPin = 3;
int pwmValue = 0;
 
// Set the Pin to output to write the PWM signal. 
void setup() 
{ 
  pinMode(pwmPin, OUTPUT);   // sets the pin as output
} 


// The main loop reads the value of the potentiometer
// then scales it appropriately for analogWrite() with map(). 
// analogWrite() outputs values between 0 and 255.
void loop() 
{ 
  for (pwmValue=1; pwmValue<256; pwmValue++){
    analogWrite(pwmPin, pwmValue);                  
    delay(5000);
  }
}


