/*

Sage Ridge Robotics
Example 15

We are going to replace the notes listed in pitches.h with scaled values from 
a potentiometer here. Potentiometer values are rescaled to a particular
range of frequencies in Hertz. One limitation of of this is the nature
of the analog to digital conversion which results in 1024 discrete
potentiometer values. This sketch also sends the current frequency over 
the serial connection at the same time that it is played.

This code is in the public domain.

*/

// speakerPin is set to digital pin 12. The piezoelectric speaker should be
// connected to this pin. The piezo' other pin should be connected to ground.
int speakerPin = 12;
int potPin01 = 0;
int potVal01;

// We open a serial connection.
void setup() {
  Serial.begin(9600);
}

// Read analog values from potentiometers to set tones.
void loop() { 
  
  potVal01 = analogRead(potPin01);
  potVal01 = map(potVal01, 0, 1023, 40, 10000);
  tone(speakerPin, potVal01,1000);
  Serial.println(potVal01);
  int pauseBetweenNotes = 500;
  delay(pauseBetweenNotes);
  noTone(speakerPin);

}

// What is the heighest note you can here? The lowest? Does duation matter?
// What about the pause between notes? What happens if you change its value?
