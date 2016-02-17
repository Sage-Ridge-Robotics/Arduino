/*

Sage Ridge Robotics
Example 16

We are going to replace the potentiometer with a photocell to create
a light-based version of a theremin. Theremins were invented by Leon
Theremin in the Soviet Union in 1926 and normally detect the electical 
field around the player. They became common elements of SciFi movie music. 

Example after Monk, Simon. Arduino Lesson 10. New York: 
Adafruit Industries, LLC, 2012.

This code is in the public domain.

*/

// speakerPin is set to digital pin 12. The piezoelectric speaker should be
// connected to this pin. The piezo' other pin should be connected to ground.
int speakerPin = 12;
int photoPin01 = 0;
int photoVal01;

// Noting to set up.
void setup()

// We loop through taking readings from the photocell, scaling the reading,
// and setting the piezo tone.
void loop() { 
  
  int reading = analogRead(photoPin01); 
  int pitch = 100 + reading / 4; 
  tone(speakerPin, pitch);
  Serial.println(reading);

}

// Play music!
