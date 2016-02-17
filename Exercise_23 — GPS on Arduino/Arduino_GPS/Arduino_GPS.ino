/*

Sage Ridge Robotics
Example 23

This sketch is modified from an Adafruit example and
implements the Adafruit GPS class and receiver.

Set a GeoCache location at t_latitude and t_longitude below, then try to find
the target location with the GPS system! This script will light three LEDs based
on proximity to the target.

*/

// LOAD LIBRARIES ---------------------------------------------------------------------------------------------

// GPS class and software serial solution
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>

// avr math library
// This is required for converting distances in Latitude and Longitude to distance meters.
#include <math.h>

// If you're using the Adafruit GPS shield, ensure 
// software serial reads SoftwareSerial mySerial(8, 7)
// and make sure the switch is set to SoftSerial
SoftwareSerial mySerial(8, 7);

// Instantiate an object of the GPS class passing mySerial
// to it.
Adafruit_GPS GPS(&mySerial);

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// We will write specific information to the serial monitor
// Set to 'true' if you want to debug and listen to the raw GPS sentences. 
#define GPSECHO false

// We will not use the interrupt approach
boolean usingInterrupt = false;
void useInterrupt(boolean);

// Target location in lat and long
float t_latitude = 39.543498;
float t_longitude = -119.875442;

// Setting things up, including the interrupt
void setup()  
{
    
  Serial.begin(115200);
  Serial.print("Start setup . . . ");
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  //GPS.sendCommand(PGCMD_ANTENNA);
  useInterrupt(true);
  Serial.println("Setup complete");
  
}


// Interrupt is called once a millisecond, looks for any new GPS data, and stores it
SIGNAL(TIMER0_COMPA_vect) {
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
#ifdef UDR0
  if (GPSECHO)
    if (c) UDR0 = c;  
    // writing direct to UDR0 is much much faster than Serial.print 
    // but only one character can be written at a time. 
#endif
}

void useInterrupt(boolean v) {
  if (v) {
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function above
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
    usingInterrupt = true;
  } else {
    // do not call the interrupt function COMPA anymore
    TIMSK0 &= ~_BV(OCIE0A);
    usingInterrupt = false;
  }
}

uint32_t timer = millis();

// The main loop
void loop(){
  
  // in case you are not using the interrupt above, you'll
  // need to 'hand query' the GPS, not suggested :(
  if (! usingInterrupt) {
    // read data from the GPS in the 'main loop'
    char c = GPS.read();
    // if you want to debug, this is a good time to do it!
    if (GPSECHO)
      if (c) Serial.print(c);
  }
  
  if (GPS.newNMEAreceived()) {
    if (!GPS.parse(GPS.lastNMEA()))
      return;
  }

  // if millis() or timer wraps around, we'll just reset it
  if (timer > millis())  timer = millis();

  // approximately every 2 seconds or so, print out the current stats
  if (millis() - timer > 5000) { 
    timer = millis(); // reset the timer
    
    Serial.print("\nTime (GMT): ");
    Serial.print(GPS.hour, DEC); Serial.print(':');
    Serial.print(GPS.minute, DEC); Serial.print(':');
    Serial.print(GPS.seconds, DEC); Serial.print('.');
    Serial.println(GPS.milliseconds);
    Serial.print("Date: ");
    Serial.print(GPS.day, DEC); Serial.print('/');
    Serial.print(GPS.month, DEC); Serial.print("/20");
    Serial.println(GPS.year, DEC);
    Serial.print("Fix: "); Serial.print((int)GPS.fix);
    Serial.print(" quality: "); Serial.println((int)GPS.fixquality); 
    if (GPS.fix) {
      
      digitalWrite(11,LOW);
      digitalWrite(12,LOW);
      digitalWrite(13,LOW);

      float c_latitude = GPS.latitudeDegrees;
      float c_longitude = GPS.longitudeDegrees;
      
      Serial.print("Location in decimal degrees: ");
      Serial.print(c_latitude, 6);
      Serial.print(", "); 
      Serial.println(c_longitude, 6);
      Serial.print("Altitude in meters: "); Serial.println(GPS.altitude);
      
      Serial.print("Speed in km/h: "); Serial.println(GPS.speed/0.539956803456);
      Serial.print("Degrees east of true north: "); Serial.println(GPS.angle);
      Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
      
      if ( abs(c_latitude - t_latitude) < 0.00001 && abs(c_longitude - t_longitude) < 0.00001 ) {
        Serial.println("Wow, you're close!"); 
        digitalWrite(13, HIGH); 
        delay(1000); 
        digitalWrite(13, LOW);
      } else {
        Serial.print("Bummer, still not there yet!"); 
        Serial.print(" Longitude difference: "); 
        Serial.print(abs(c_longitude - t_longitude),6); 
        Serial.print(" Latitude difference: "); 
        Serial.println(abs(c_longitude - t_longitude),6);
      }
      
      if ( abs(c_latitude - t_latitude) < 0.0001 && abs(c_longitude - t_longitude) < 0.0001 ) {
        Serial.println("Geting closer!"); 
        digitalWrite(12, HIGH); 
        delay(1000); 
        digitalWrite(12, LOW);
      } else {
        Serial.print("Still warm, but that is just about it"); 
        Serial.print(" Longitude difference: "); 
        Serial.print(abs(c_longitude - t_longitude),6); 
        Serial.print(" Latitude difference: "); 
        Serial.println(abs(c_longitude - t_longitude),6);
      }

      if ( abs(c_latitude - t_latitude) < 0.001 && abs(c_longitude - t_longitude) < 0.001 ) {
        Serial.println("Warm"); 
        digitalWrite(11, HIGH); 
        delay(1000); 
        digitalWrite(11, LOW);
      } else {
        Serial.print("Not even warm!"); 
        Serial.print(" Longitude difference: "); 
        Serial.print(abs(c_longitude - t_longitude),6); 
        Serial.print(" Latitude difference: "); 
        Serial.println(abs(c_longitude - t_longitude),6);
      }

     // Distance to target cache
     // Spherical Law of Cosines approximation of distance between two geographic points.
     // For more accuracy, use the Haversine formula
       // a = sin²(Δφ/2) + cos φ1 ⋅ cos φ2 ⋅ sin²(Δλ/2)
       // c = 2 ⋅ atan2( √a, √(1−a) )
       // d = R ⋅ c 
       // Where R is is earth’s radius (mean radius = 6371000m)
     tr_latitude = t_latitude * (PI/180);  
     cr_latitude = c_latitude * (PI/180);
     tr_longitude = t_longitude * (PI/180);
     cr_longitude = c_longitude * (PI/180);
     float t_distance = acos( sin(cr_latitude) * sin(tr_latitude) + cos(cr_latitude) * cos(tr_latitude) * cos(tr_longitude - cr_longitude) ) * 6371000;
     Serial.print("Distance from target cache"); Serial.print(t_distance); Serial.println("m");
     
     // Forward azimuth or initial bearing
     // θ = atan2( sin Δλ ⋅ cos φ2 , cos φ1 ⋅ sin φ2 − sin φ1 ⋅ cos φ2 ⋅ cos Δλ )
     float y = sin(tr_latitude - cr_longitude) * cos(tr_latitude);
     float x = cos(cr_latitude) * sin(tr_latitude) -
     sin(cr_latitude) * cos(tr_latitude) * cos(tr_longitude - cr_longitude);
     float bearing = (atan2(y, x) * (180/PI));
     Serial.print("Forward azimuth"); Serial.print(t_distance); Serial.println("degrees east of true north");
     
     
     if ( t_distance < 10)  {digitalWrite(13,HIGH);}
     if ( t_distance < 50)  {digitalWrite(12,HIGH);}
     if ( t_distance < 200) {digitalWrite(11,HIGH);}
      
    }
  }
}

