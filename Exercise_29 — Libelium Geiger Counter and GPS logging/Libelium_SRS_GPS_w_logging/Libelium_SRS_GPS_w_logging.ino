/*  
 *  Geiger Counter - Radiation Sensor Board w. GPS and logging
 * 
 *  This sketch provides GPS data for readings and logs the data to
 *  a micro SD card. You will need the Adafruit GPS Arduino shield
 *  and an external GPS antenna for this sketch to work correctly. 
 *  Data is also sent over serial. 
 *
 *  Note: Due to pin conflicts, the LED array and LCD screen are not used.
 *
 *  Original copyright (C) Libelium Comunicaciones Distribuidas S.L. 
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
 *  Version:                    1.0
 *  Original design:            David Gascón 
 *  Original implementation:    Marcos Yarza
 *  GPS and logging extensions  Christopher von Nagy
 */

// GPS class and software serial solution
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>

// SD card library
#include <SPI.h>
#include <SD.h>

#include <avr/sleep.h>

// GPS
// If you're using the Adafruit GPS shield, ensure 
// software serial reads SoftwareSerial mySerial(8, 7)
// and make sure the switch on the board is set to SoftSerial.
SoftwareSerial mySerial(8, 7);

// Instantiate an object of the GPS class passing mySerial
// to it. Serial over pins 0 and 1 (USB) will continue to work.
Adafruit_GPS GPS(&mySerial);

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// We will write specific information to the serial monitor
// Set to 'true' if you want to debug and listen to the raw GPS sentences but
// then comment out all the code below to write data over serial.
#define GPSECHO false

// Setup SD
// Note: On the Arduino UNO the SD system uses pins 11 (MOSI), 
// 12 (MISO), 13 (CLK), 4 (CS), and 10 (SS). Communication is over
// the SPI protocol
#define chipSelect 10
#define ledPin 13

// Instantiate a file object for reading and writing to SD card
File dataFile;


// Conversion factor - CPM to uSV/h
// The figure used below is very ballpark and is taken from RHElectronics for the old Soviet SI-180G
// tube used in the example. If you are employing a different tube, then you should uncomment the
// corresponding conversion factor and comment out the factor for the SI-180G tube. If your tube is not
// listed here, then you must located the conversion factor on the spec sheet or via the Internet.

// Credit goes to RHElectronics for the values.
// Note: The SI-180G tube is very, very good for background radiation detection.

#define CONV_FACTOR 0.0031 // SI-180G

// define CONV_FACTOR 0.008120 // J305ß 
// #define CONV_FACTOR 0.0057 // SBM-20 
// #define CONV_FACTOR 0.0021 //SBM-19 
// #define CONV_FACTOR 0.0082 // SI-29BG 
// #define CONV_FACTOR 0.0081 //LND-712 
// #define CONV_FACTOR 0.0024 // LND-7317 
// #define CONV_FACTOR 0.0031 // SBT11-A 
// #define CONV_FACTOR 0.0117 // SBT-9 

// Variables
int geiger_input = 2;               // Input from the Libelium sheild is on 2
long count = 0;
long countPerMinute = 0;
long timePrevious = 0;
long timePreviousMeassure = 0;
long time = 0;
long countPrevious = 0;
float radiationValue = 0.0;
float c_latitude = 0.0;
float c_longitude = 0.0; 
float c_altitude = 0.0; 


void setup(){
  
  // Set pin modes for logging system
  pinMode(ledPin, OUTPUT);
  pinMode(chipSelect, OUTPUT);


  // Set up to use pin 2 for geiger 'ticks'  
  pinMode(geiger_input, INPUT);
  digitalWrite(geiger_input,HIGH);

  // Start serial link to machine (not soft serial)
  Serial.begin(115200);
  Serial.println("\nStarting ...");
  

  // Initialize card or stop
  Serial.print("Initializing SD card ...");
  if (!SD.begin(chipSelect, 11, 12, 13)) {
    Serial.println("Card init. failed!");
    error(2);
  }
  else {Serial.println("Card initialized.");}

  // Create a new file name each instance  
  char filename[15];
  strcpy(filename, "RADLOG00.TXT");
  for (uint8_t i = 0; i < 100; i++) {
    filename[6] = '0' + i/10;
    filename[7] = '0' + i%10;
    if (! SD.exists(filename)) {
      break;
    }
  }
  
  // Open file for writing or stop
  dataFile = SD.open(filename, FILE_WRITE);
  if( ! dataFile ) {
    Serial.print("Couldnt create "); 
    Serial.println(filename);
    error(3);
  } else {
    Serial.print("Writing to "); 
    Serial.println(filename);
  }
  
  // Start GPS
  GPS.begin(9600);                               // Soft serial link at 9600
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);  // turn on GPRMC and GGA
  GPS.sendCommand(PMTK_API_SET_FIX_CTL_1HZ);     // Queries for fix at 1 Hz
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);     // Updates NMEA at 1 Hz
  // GPS.sendCommand(PMTK_ENABLE_SBAS);          // Enable SBAS 
  GPS.sendCommand(PMTK_ENABLE_WAAS);             // Enable WAAS parsing for improved accuracy
                                                 // Only works in North America
  GPS.sendCommand(PGCMD_NOANTENNA);                // Turn off updates on antenna status
    
  // Intialize interrupt for the Libelium shield
  // The sketch uses the interrupt mechanism on digital
  // pin 2 to receive 'ticks' from the Libellium board.

  // pin 2 is interrupt 0
  // countPulse is the function to call when a pulse happens
  // The mode FALLING means that we are listening for a switch
  // from HIGH to LOW (digital on to digital off).
  attachInterrupt(0,countPulse,FALLING);
   

}


void loop(){
  
  // We use dataString for logging and reporting over serial.  
  String dataString = "";
  
  // We output radiation data every 10 seconds.
  if (millis()-timePreviousMeassure > 10000){

    // GPS data
    // See if we have a NMEA GPS sentence and parse if we do.
    if (GPS.newNMEAreceived()) {GPS.parse(GPS.lastNMEA());}

    // If we have a fix, get the GPS coordinates and elevation. If not, just set to 0.0    
    if (GPS.fix) {
      c_latitude  = GPS.latitudeDegrees;
      c_longitude = GPS.longitudeDegrees;
      c_altitude  = GPS.altitude;
    } else {
      c_latitude  = 0.0;
      c_longitude = 0.0; 
      c_altitude  = 0.0; 
    } 


    // Radiation data
    countPerMinute = 6*count;
    radiationValue = countPerMinute * CONV_FACTOR;
    timePreviousMeassure = millis();
   
    // Compose dataString with GPS and radiation data.
    dataString += String((int)GPS.fix);
    dataString += String(",");
    dataString += String((int)GPS.fixquality); 
    dataString += String(",");
    dataString += String((int)GPS.year);
    dataString += String("-");
    dataString += String((int)GPS.month);
    dataString += String("-");
    dataString += String((int)GPS.day);
    dataString += String(",");
    dataString += String((int)GPS.hour);
    dataString += String(":");
    dataString += String((int)GPS.minute);
    dataString += String(":");
    dataString += String((int)GPS.seconds);
    dataString += String(".");
    dataString += String((int)GPS.milliseconds);
    dataString += String(",");
    dataString += String(c_latitude);
    dataString += String(",");
    dataString += String(c_longitude);
    dataString += String(",");
    dataString += String(c_altitude);
    dataString += String(",");
    dataString += String(countPerMinute);
    dataString += String(",");
    dataString += String(radiationValue);
    
    // Write dataString to serial and to the SD card
    dataFile.println(dataString);
    dataFile.flush();    
    Serial.println(dataString);

    count = 0;
    

  }

}

// Function run when the interrupt receives a signal from the 
// Libelium geiger counter board.
void countPulse(){
  detachInterrupt(0);
  count++;
  while(digitalRead(2)==0){
  }
  attachInterrupt(0,countPulse,FALLING);
}

// Function to blink out an error code
// From Ladyada's logger sketch
void error(uint8_t errno) {
  while(1) {
    uint8_t i;
    for (i=0; i<errno; i++) {
      digitalWrite(ledPin, HIGH);
      delay(100);
      digitalWrite(ledPin, LOW);
      delay(100);
    }
    for (i=errno; i<10; i++) {
      delay(200);
    }
  }
}
