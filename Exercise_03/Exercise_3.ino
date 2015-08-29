/*

Sage Ridge Robotics
Example 03
A slightly modified Sketch from the Arduino Blink example

Turns multiple LEDs on then off in sequence.
This example code is in the public domain.

*/
 
// Here we declare initial variables. In Example 03 we will use
// variablew led01, led02, and led03. We want to use these variables to
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
// reset or powered down. Here we set the voltage HIGH on led01, led02, and led03
// then LOW in sequence causing the connected LEDs to blink one after another.
// The delay between blinks is set to one quarter of a second.
void loop() {
  digitalWrite(led01, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(250);                  // wait for a second
  digitalWrite(led01, LOW);    // turn the LED off by making the voltage LOW
  delay(250);                  // wait for a second
  digitalWrite(led02, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(250);                  // wait for a second
  digitalWrite(led02, LOW);    // turn the LED off by making the voltage LOW
  delay(250);                  // wait for a second
  digitalWrite(led03, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(250);                  // wait for a second
  digitalWrite(led03, LOW);    // turn the LED off by making the voltage LOW
  delay(250);                  // wait for a second
}
