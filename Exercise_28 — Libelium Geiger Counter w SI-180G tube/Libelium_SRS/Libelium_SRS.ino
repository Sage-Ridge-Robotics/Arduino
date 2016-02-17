/*  
 *  Geiger Counter - Radiation Sensor Board 
 *
 *  This sketch displays radiation data on an LCD screen. It 
 *  also sends data over serial. You might create a Processing
 *  or Mathematica program to parse and handle this data. If you
 *  do so, consider modifying the manner in which the data is set. 
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
 *  Original design:            David Gascón 
 *  Original implementation:    Marcos Yarza
 *  Changes                     Christopher von Nagy
 *
 */

// include the LiquidCrystal library code:
#include <LiquidCrystal.h>

// LiquidCrystal
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(3,4,5,6,7,8);

// Threshold values for the led bar
// These threshold values are counts
// per minute.
#define TH1 45
#define TH2 95
#define TH3 200
#define TH4 400
#define TH5 600

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

// Variables
int ledArray [] = {10,11,12,13,9};
int geiger_input = 2;
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

  Serial.begin(115200);
  Serial.println("\nStarting ...");
  
  //set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Radiation Sensor");
  lcd.setCursor(0,1);
  lcd.print("SI-180G tube");  
  delay(5000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SRS Robotics");
  lcd.setCursor(0,1);  
  lcd.print("Go Scorpions!");
  delay(5000);

  lcd.clear();  
  lcd.setCursor(0, 0);
  lcd.print("CPM=");
  lcd.setCursor(4,0);
  lcd.print(6*count);
  lcd.setCursor(0,1);
  lcd.print(radiationValue);
  
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
   
    // Reader friendly 
    Serial.print("cpm = "); 
    Serial.print(countPerMinute,DEC);
    Serial.print(" ");
    Serial.print("uSv/h = ");
    Serial.println(radiationValue,4);
    
    // Logging friendly
    // Serial.print(countPerMinute,DEC);
    // Serial.print(",");
    // Serial.println(radiationValue,4);
    
    lcd.clear();    
    lcd.setCursor(0, 0);
    lcd.print("CPM=");
    lcd.setCursor(4,0);
    lcd.print(countPerMinute);
    lcd.setCursor(0,1);
    lcd.print(radiationValue,4);
    lcd.setCursor(6,1);
    lcd.print(" uSv/h");

    //led var setting  
    if(countPerMinute <= TH1) ledVar(0);
    if((countPerMinute <= TH2)&&(countPerMinute>TH1)) ledVar(1);
    if((countPerMinute <= TH3)&&(countPerMinute>TH2)) ledVar(2);
    if((countPerMinute <= TH4)&&(countPerMinute>TH3)) ledVar(3);
    if((countPerMinute <= TH5)&&(countPerMinute>TH4)) ledVar(4);
    if(countPerMinute>TH5) ledVar(5);

    count = 0;
    

  }

}

void countPulse(){
  detachInterrupt(0);
  count++;
  while(digitalRead(2)==0){
  }
  attachInterrupt(0,countPulse,FALLING);
}

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

  
