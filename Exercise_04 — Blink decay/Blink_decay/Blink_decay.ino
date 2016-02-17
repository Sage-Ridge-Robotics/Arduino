/*

Sage Ridge Robotics
Example 04
A slightly modified Sketch from the Arduino Blink example

This sketch blinks an LED at increasingly shorter intervals.
This example code is in the public domain.

*/
 
// Here we declare initial variables. In Example 04 we will just use
// variable led01. We want to use these variables to declare which 
// output pins we intend to use. We also create a variable called delay01
// to hold the value of the delay to use.
// give it a name:
int led01 = 13;
int delay01 = 1000;

// The setup function runs once when you press reset or load a sketch.
// We need to set the pins to output mode. Pins may either be in
// output or input mode.

void setup() {                
  pinMode(led01, OUTPUT);     
}

// The loop function runs continuously until the Arduino board is 
// reset or powered down. Here we turn an LED connected to pin 13 on and
// off at an increasingly faster rate. We change the rate by 25 milliseconds
// each time we loop through until we reach zero. At that point, the delay is
// reset to 1000 milliseconds.
void loop() {
  
  digitalWrite(led01, HIGH);       // The on and off sequence
  delay(delay01);                  
  digitalWrite(led01, LOW);    
  delay(delay01);      
  
  delay01 = delay01 - 25;         // We shorten the delay
  if (delay01 == 0){              // We check if the delay is zero
    delay01 = 1000;               // If delay01 is zero, then we reset to 1000
  }                               // Notice how curly brackets are used
  
}
