/*

Sage Ridge Robotics
Example 02
A slightly modified Sketch from the Arduino Blink example

Turns on multiple LEDs on for one second, then off for one second, repeatedly.
This example code is in the public domain.

*/
 
// Here we declare initial variables. In Example 02 we will use
// variables led01, led02, and led03. We want to use these variables to 
// declare which output pins we intend to use.
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
// reset or powered down. Here set the voltage HIGH on led01, led02, and led03
// turning the connected LEDs on. Then we set the pins LOW to turn them off.
// Delay is set to 1000 milliseconds or 1 second.
void loop() {
  digitalWrite(led01, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(led02, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(led03, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(led01, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(led02, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(led03, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);               // wait for a second
}
