/*  
 *  Geiger Counter - Radiation Sensor Board 
 *
 *  This sketch handles radiation data and sends a simple 
 *  stream of comma-delimited data over the serial link. The
 *  display is not used.
 *  
 *  Copyright (C) Libelium Comunicaciones Distribuidas S.L. 
 *  http://www.libelium.com 
 *  
 *  Copyright (C) 2016 Christopher von Nagy
 *  cvonnagy@sageridge.org
 *
 *  This program is free software: you can redistribute it and/or modify 
 *  it under the terms of the GNU General Public License as published by 
 *  the Free Software Foundation, either version 3 of the License, or 
 *  (at your option) any later version. 
 *  a
 *  This program is distributed in the hope that it will be useful, 
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License 
 *  along with this program.  If not, see http://www.gnu.org/licenses/. 
 *  
 *  Version:           1.0
 *  Author             Christopher von Nagy
 *  Libelium elements: David Gascón and Marcos Yarza 
 *
 */

// Conversion factor - CPM to uSV/h
// Converting counts per minute to uSV/h (absorbed radiation) is a complicated matter.
// The figure for the old Soviet SI-180G Geiger Muller tube used here is very ballpark 
// and is taken from RHElectronics documentation for the tube.
// If you are employing a different tube, then you should uncomment the
// corresponding conversion factor and comment out the factor for the SI-180G tube. 
// If your tube is not listed here, then you must located the conversion factor on the 
// spec sheet or via the Internet.

// Credit goes to RHElectronics for the values listed here.

// Note: The SI-180G tube is very, very good for background radiation detection.

#define CONV_FACTOR 0.0031 // SI-180G

// #define CONV_FACTOR 0.008120 // J305ß 
// #define CONV_FACTOR 0.0057 // SBM-20 
// #define CONV_FACTOR 0.0021 //SBM-19 
// #define CONV_FACTOR 0.0082 // SI-29BG 
// #define CONV_FACTOR 0.0081 //LND-712 
// #define CONV_FACTOR 0.0024 // LND-7317 
// #define CONV_FACTOR 0.0031 // SBT11-A 
// #define CONV_FACTOR 0.0117 // SBT-9 

// Threshold values for the led bar
// These threshold values are counts
// per minute.
#define TH1 45
#define TH2 95
#define TH3 200
#define TH4 400
#define TH5 600



// Variables
int ledArray [] = {10,11,12,13,9};

int geiger_input = 2;
int ledLevel;
long count = 0;
long countPerMinute = 0;
long timePrevious = 0;
long timePreviousMeassure = 0;
long time = 0;
long countPrevious = 0;
float radiationValue = 0.0;

void setup(){

  pinMode(geiger_input, INPUT);
  digitalWrite(geiger_input,HIGH);

  for (int i=0;i<5;i++){
    pinMode(ledArray[i],OUTPUT);
  }

  Serial.begin(9600);
  
  // The sketch uses the interrupt mechanism on digital
  // pin 2 to receive 'ticks' from the Libellium board.

  // pin 2 is interrupt 0
  // countPulse is the function to call when a pulse happens
  // The mode FALLING means that we are listening for a switch
  // from HIGH to LOW (digital on to digital off).
  // Interrupts allow the Arduino to respond immediately to events
  // on the interrupt pin regardless of the state of execution of
  // codes.
  attachInterrupt(0,countPulse,FALLING);

}

void loop(){

  // We output radiation data every 10 seconds.  
  if (millis()-timePreviousMeassure > 10000){

    // Radiation data
    countPerMinute = 6*count;
    radiationValue = countPerMinute * CONV_FACTOR;
    timePreviousMeassure = millis();
   
    //led var setting  
    if(countPerMinute <= TH1) ledVar(0);
    if((countPerMinute <= TH2)&&(countPerMinute>TH1)) { ledVar(1); ledLevel = 1; }
    if((countPerMinute <= TH3)&&(countPerMinute>TH2)) { ledVar(2); ledLevel = 2; }
    if((countPerMinute <= TH4)&&(countPerMinute>TH3)) { ledVar(3); ledLevel = 3; }
    if((countPerMinute <= TH5)&&(countPerMinute>TH4)) { ledVar(4); ledLevel = 4; }
    if(countPerMinute>TH5) ledVar(5);

    // Data parsing friendly 
    Serial.print(countPerMinute);
    Serial.print(",");
    Serial.print(radiationValue,4);
    Serial.print(",");
    Serial.print(ledLevel);
    Serial.println("\0");                     // line terminator

    count = 0;
    

  }

}


// Function to count the pulses
void countPulse(){
  detachInterrupt(0);
  count++;
  while(digitalRead(2)==0){
  }
  attachInterrupt(0,countPulse,FALLING);
}

// Function to illuminate LEDs depending on defined transition points
// in the radiation level.
void ledVar(int value){
  if (value > 0){
    for(int i=0;i<=value;i++){
      digitalWrite(ledArray[i],HIGH);
    }
    for(int i=5;i>value;i--){
      digitalWrite(ledArray[i],LOW);
    }
  }
  else {
    for(int i=5;i>=0;i--){
      digitalWrite(ledArray[i],LOW);
    }
  }
}

  
