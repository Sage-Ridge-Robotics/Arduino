/*
Sage Ridge Robotics
Example 6

ElectroSketch, or how to control a brush on the canvas with two 
potentiometers. This Arduino sketch pairs with Processing Example
sketch 6. The Arduino handles input from the transducers (potentio-
meters, etc) and the Processing sketch handles the display.

You might consider adding controls to reset the canvas, set the color
and line width of the brush, etc. Use this sketch and the paired
Processing sketches as starting points.

Adapted from Processing.org

*/

// Here we declare the pin numbers for potentiometers 0 and 1
// Pot 0 will control the horizontal, x, while Pot 1 the vertical, y.
const int potPin0 = 0; // const signals to the compiler that
const int potPin1 = 2; // the values of these variables cannot change.
int potVal0 = 100;
int potVal1 = 100;
// int foo. Remember to declare any new variables that you create.

// In setup() we open the serial output for streaming 
// at a rate of 9600 characters per second
void setup() 
{
  Serial.begin(9600);
}

// In the main loop we read the analog value of potPin0 and 
// potPin1 every 250 milliseconds and stream the 
// values out over the serial connection as x,y pairs. Each pair 
// is followed by an ASCII new line character, "\0".
// map(Min, Max, newMin, newMax) rescales the output values. 
// Here 500 is used as the new maximum in map to conform to the
// canvas size in the Processing Elektro-Sketch example.
void loop() 
{

  potVal0 = analogRead(potPin0);             // The wiper (middle) pin from the first potentiometer goes here.
  potVal1 = analogRead(potPin2);             // The wiper (middle) pin from the second potentiometer goes here.
  potVal0 = map(potVal0, 0, 1023, 0, 500);   // map converts a range of values to another range of values.
  potVal1 = map(potVal1, 0, 1023, 0, 500);   // Here we take the range of 0..1024 from the on-board analog to
                                             // digital converter on the Arduino and map it to 0 .. 500 for
                                             // use in our Processing sketch. If you change the size of your canvas
                                             // in Processing, you will possibly want to change the value here.

  // Send the remapped digital values from the potentiometers to the Processing 
  // sketch via the serial link. 
  
  Serial.print(potVal0);                     // x value
  Serial.print(",");
  Serial.print(potVal1);                     // y value
  // Serial.print(",");
  // Serial.print(foo);                      // another variable. Perhaps for color?
  
  Serial.println(",\0");                     // Add an line terminator to make the Processing sketch more happy. 
  delay(250);                                // Set to a shorter delay for faster response.
                                             // Experiment.

}
 
