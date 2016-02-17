/*

Sage Ridge Robotics
Example 7
Controlling a servo with a potentiometer
and reporting the value over the serial link

Code adapted from Arduino.cc and Michal Rinott <http://people.interaction-ivrea.it/m.rinott>

*/

// Load the servo.h library  
// Notice the syntax used in include a library in Arduino (C++)
#include <Servo.h> 
 
// create in instance (object) of the servo class to control a physical servo
Servo myServo0;   
 
// Create integer variables for the potentiometer and servo:
// potPin is set to analog pin A0 on the Arduino board
// potVal is also declared. It will hold values read 
// from analog pin A0.
// servoPin  is set to digital pin 9 on the board.
int potPin = 0;
int potVal;
int servoPin = 9;
 
// Open the serial port and attach the servo object, 
// myServo0, to digital pin 9. This is where we 
// attached the physical servo' control wire (yellow).
void setup() 
{ 
  Serial.begin(9600);
  myServo0.attach(servoPin);  
} 


// The main loop reads the value of the potentiometer
// then scales it appropriately for the servo. The servo 
// requires a value between 0 and 180 degrees. The code
// then sets the servo position according to the scaled value, 
// sends the value out over the serial link,
// and waits for the servo to move to the desired position. 
void loop() 
{ 
  potVal = analogRead(potPin);
  potVal = map(potVal, 0, 1023, 0, 179);     
  myServo0.write(potVal);                  
  Serial.println(potVal);  
  delay(2000);                           
}


