/*
Sage Ridge Robotics
Example 9

This Processing sketch is intended to pair with Arduino 
Exercise 20. It distabce data over the  serial link 
from an Ardinoattached to an ultrasonic sonar sensor.
This sketch displays the reported distance data
graphically.

Make sure that you have mapped the distance to 0 . 255
in the Arduino sketch.

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
String portName = "/dev/tty.usbmodemfa141";  // Set to correct serial port
int lf = 10;    // Linefeed in ASCII

// Set the canvas size, color mode (hue, saturation, value),
// aliasing, drawing stroke, background color, 
// and instantiate myPort. Clear any possible garbage
// in the serial stream.
void setup() 
{
  
  size(600, 600);
  noStroke();
  background(0);
  
  myPort = new Serial(this, portName, 9600);
  myPort.clear();

}

// 
void draw() {
  while (myPort.available() > 0) {
    String junk = myPort.readStringUntil(lf);   
    String inBuffer = myPort.readStringUntil(lf);
    if (inBuffer != null) {
      background(0);
      int esLux[] = int(split(inBuffer,','));
      println(inBuffer + "\n");
      background(int(esLux[1]));      }
  } 
}


