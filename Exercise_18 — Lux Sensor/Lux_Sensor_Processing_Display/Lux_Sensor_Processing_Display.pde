/*
Sage Ridge Robotics
Example 7

This Processing sketch is intended to pair with Arduino 
Exercise 18. t receives luminosity data in lux over the 
serial link from an Arduino attached to a luminosity sensor.
The Arduino sketch can be configured to report visible
luminosity, infrared luminosity, or total luminosity.
This sketch displays the reported luminosity graphically.

Adapted from Processing.org
This code is in the public domain.

*/

// Import the serial library. Unlinke Arduino, serial 
// communication functions are in a loadable library
// in Processing.
import processing.serial.*;


// Declare an object of the Serial class. The serial port 
// needs to be manually defined; here it is an OSX port.
// For Windows, use the COM port the Arduino board is attached to.
Serial myPort;
String inBuffer = null;      
String portName = "/dev/tty.usbmodemfa141"; 
int lf = 10;    // Linefeed in ASCII

// Set the canvas size, color mode (hue, saturation, value),
// aliasing, drawing stroke, background color, 
// and instantiate myPort. Clear any possible garbage
// in the serial stream.
void setup() 
{
  
  size(600, 600);
  colorMode(HSB, 1000, 1000, 1000);  
  noStroke();
  background(0);
  
  myPort = new Serial(this, portName, 9600);
  myPort.clear();

}

// Read the reported lux value and draw an ellipse sized 
// and colored baed on this value. Lux data is used for hue.
void draw() {
  while (myPort.available() > 0) {
    String inBuffer = myPort.readStringUntil(lf);
    if (inBuffer != null) {
      background(0);
      int esLux = int(inBuffer);
      fill(int(esLux), 500, 750);
      ellipse(height/2, width/2, esLux, esLux);
    } 
  }
}


