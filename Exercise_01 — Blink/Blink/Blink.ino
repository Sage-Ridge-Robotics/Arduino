/*

Sage Ridge Robotics
Example 01
A slightly modified Sketch from the Arduino Blink example

Turns on an LED on for one second, then off for one second, repeatedly.
This example code is in the public domain.

*/
 
// Here we declare initial variables. In Example 01 we will just use
// variable led01. We want to use these variables to declare which 
// output pins we intend to use.
// give it a name:
int led01 = 13;
int led02 = 12;
int led03 = 11;

// The setup function runs once when you press reset or load a sketch.
// We need to set the pins to output mode. Pins may either be in
// output or input mode.

void setup() {                
  pinMode(led01, OUTPUT);     
  pinMode(led02, OUTPUT);     
  pinMode(led03, OUTPUT);     
}

// The loop function runs continuously until the Arduino board is 
// reset or powered down. Here we flash led01 on and off with a
// 1000 millisecond delay.
void loop() {
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);               // wait for a second
}
