/*
Sage Ridge Robotics
Example 6 — ElectroSketch

ElecktroSketch, or how to control a brush on the 
canvas with two potentiometers. This Processing sketch
is paired with Arduino Exercise 6.

Add more potentiometers, switches, or other gizmos to your 
Arduino to control color, line width, etc!

Adapted from Processing.org

*/

// Import the serial library. Unlinke Arduino, serial 
// communication functions are in a loadable library
// in Processing.
import processing.serial.*;


// Instantiate objects of the Serial and esPen classes
// Data received from the serial port. esPen is defined at
// the end of this sketch. The serial port needs to be
// manually defined. The exmaple here is for OSX (Mac).
esPen myPen;
Serial myPort;
color initColor = color(50,50,50); // The RGB pen color
String inBuffer = null;      
String portName = "/dev/tty.usbmodemfa141"; // Set to the correct tty (OSX, Linux) or COM (Windows) port
int lt = 10;

// Set the canvas size, color mode (hue, saturation, value),
// aliasing, drawing stroke, background color, 
// and open the serial port|file.
// The clear method of the serial class ensures that we receive
// a complete line of values from the Arduino the first time.
void setup() 
{
  size(500, 500);
  colorMode(HSB, 100);
  noSmooth();
  strokeWeight(3);
  background(0,0,100);

  myPen = new esPen(250,250,initColor);
  myPort = new Serial(this, portName, 9600);
  myPort.clear();
}

// Processing using draw() rather than loop(). Like Arduino,
// this is an infinite loop. 

// While myPort is available we read lines of data streaming
// from the Arduino, split the character stream at commas,
// and store the data (x, y) in an array. The position of the 
// pen object, myPen, is then updated with the data streamed 
// from the Arduino.
void draw() {
  while (myPort.available() > 0) {
    String inBuffer = myPort.readStringUntil(lt);
    if (inBuffer != null) {
      int[] esPenState = int(split(inBuffer,','));

      myPen.xPrev = myPen.x;
      myPen.yPrev = myPen.y;
      myPen.x = esPenState[0];
      myPen.y = esPenState[1];
      // myPen.hueValue = esPenState[2];
      myPen.ink();

    } 
  }
}

// Declare a pen class for the Elektro-Sketch
// The pen class will know its position and the pen' hue
// Saturation and value (brightness) will be preset.
// esHSB defines the color of the pen.
class esPen {
  
 // Data to be stored in object instance
 int x;
 int y;
 int xPrev;
 int yPrev;
 int hueValue;
 int satValue;
 int brightnessValue;
 color esHSB; 

 // Constructor to create object from class definition
 // hueValue set the hue (color) of the pen. Color is
 // determined by its hue, saturation, and brightness.
 // The constructor sets inital values.
 esPen (int xpos, int ypos, int hV) {
 x = xpos;
 y = ypos;
 xPrev = xpos;
 yPrev = ypos;
 hueValue = hV; 
 satValue = 50; 
 brightnessValue = 50; 
 esHSB = color(hueValue,satValue,brightnessValue);

 }

 // Method to draw the etch point. First the fill is set to 
 // the color set by hueValue. Saturation and brightness are static
 // but can be changed here.
 void ink() {
   esHSB = color(hueValue,50,50); // Color can be changed here.
   stroke( esHSB );
   line(x, y, xPrev, yPrev);
 }
}




