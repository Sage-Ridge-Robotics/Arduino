/*

Sage Ridge Robotics
Example 17

This sketch supports the construction of a traditional theramin using
two analog inputs, one controlling pitch, the other volume. The
theramin traditionally uses your body' electric field and two antennae 
to control pitch and volume values. You can construct a traditional
version or use other input devices connected to your Arduino. The only
requirement is that you produce two analog signals with a voltage
span of 0 to 5 volts.

This sketch uses toneAC replacement tone library. This library uses pins
9 (GRD) and 10 (Voltage).

This code is in the public domain.

*/

// Load the toneAC library
#include <toneAC.h>

// The piezoelectric speaker should be connected to pin 10. 
// The piezo' other pin should be connected to ground, pin 9.
// theramin input pins are analog pins 0 and 1 (or change them!)
int theraminPin00 = 0; // Pitch
int theraminVal00;
int theraminPin01 = 1; // Volume
int theraminVal01;

// Noting to set up.
void setup() {}

// We loop through taking readings from the analog inputs, 
// scaling the readings with map and setting the speaker tone
// and volume. The map function for pitch is set to 25 Hz
// increments. Experiment with other ways to map the analog
// values 0..1023 to frequencies. Volume has 10 steps in the
// library. Modify the library for different volume behavior 
// in your theramin.
void loop() { 
  
  int pitch = map(analogRead(theraminPin00),0,1023,0,25575);  // Pitch in 25 Hz steps
  int volume = map(analogRead(theraminPin01),0,1023,0,10);    // Volume
  toneAC(pitch, volume);
  
}

// Play music!
