/*

Sage Ridge Robotics
Example 14

Sketch after Arduino.cc. This sketch uses the tone() function.
This program is in the public domain.

*/


// The piches.h file is loaded and contains tones definitions of notes.
#include "pitches.h"

// speakerPin is set to digital pin 12. The piezoelectric speaker should be
// connected to this pin. The piezo' other pin should be connected to ground.
// notes[][] is a two dimensional array containing a reference to the note
// the duration of the note, 4 is a quarter note, 8 is an eighth note, etc. 
int speakerPin = 12;
int numNotes = 11;
int notes[11][2] = {
    {NOTE_C4,4}, 
    {NOTE_G3,4}, 
    {NOTE_G3,4}, 
    {NOTE_A3,2}, 
    {NOTE_G3,1}, 
    {NOTE_C4,2},
    {NOTE_B3,8},
    {NOTE_G3,2},
    {NOTE_A3,4},
    {NOTE_G3,4},
    {NOTE_C4,4}
  };

// We want the tone sequence to play once. Therefore, we put the code in setup()
// rather than the loop() function. We will use for() loop to allow
// iteration over the list (array) of notes. To calculate the note
// length, we divide one second by the note duration number in the array, 
// notes[][]. The duration of a quarter note, 4, becomes 1000/4 or
// 250 milliseconds. 
void setup() {
 
  for (int thisNote = 0; thisNote < numNotes; thisNote++) {

    int noteDuration = 1000/notes[thisNote][1];
    tone(speakerPin, notes[thisNote][0],noteDuration);

     int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);

    noTone(speakerPin);
    
  }
}


// We play the notes once in setup().
void loop() { }

// Compose something.
