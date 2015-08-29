/*

Sage Ridge Robotics
Example 24

This sketch is modified from an Adafruit example and
implements the Adafruit GPS class and receiver.

*/

// LOAD LIBRARIES ---------------------------------------------------------------------------------------------

// GPS class and software serial solution
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>

// avr math library
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
float t_latitude = 39.409415;
float t_longitude = -119.801130;

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

  // approximately every 5 seconds or so, print out the current stats
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
      
      Serial.print("Speed in km/h: "); Serial.println(GPS.speed/1.85200);
      Serial.print("Degrees east of true north: "); Serial.println(GPS.angle);
      Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
      
   // Distance to target cache
     // Spherical Law of Cosines approximation of distance between two geographic points.
     // d = acos( sin φ1 ⋅ sin φ2 + cos φ1 ⋅ cos φ2 ⋅ cos Δλ ) ⋅ R
     // For more accuracy, use the Haversine formula
       // a = sin²(Δφ/2) + cos φ1 ⋅ cos φ2 ⋅ sin²(Δλ/2)
       // c = 2 ⋅ atan2( √a, √(1−a) )
       // d = R ⋅ c 
       // Where R is is earth’s radius (mean radius = 6371000m)
       
     float tr_latitude = radians(t_latitude);  
     float cr_latitude = radians(c_latitude);
     float tr_longitude = radians(t_longitude);
     float cr_longitude = radians(c_longitude);
     float delta_latitude = radians(t_latitude - c_latitude);
     float delta_longitude = radians(t_longitude - c_longitude);
     long  R = 6371000; // Mean radius of earth at equator in meters
     
     // float t_distance = acos( sin(cr_latitude) * sin(tr_latitude) + cos(cr_latitude) * cos(tr_latitude) * cos(delta_longitude) ) * R;
     float a = pow(sin(delta_latitude/2),2) + cos(c_latitude) * cos(t_latitude) * pow(sin(delta_longitude/2),2);
     float c = 2 * atan2(sqrt(a), sqrt(1-a));
     float t_distance = R * c;
     Serial.print("Distance from target cache: "); Serial.print(t_distance); Serial.println("m");
     
     // Forward azimuth or initial bearing
     // θ = atan2( sin Δλ ⋅ cos φ2 , cos φ1 ⋅ sin φ2 − sin φ1 ⋅ cos φ2 ⋅ cos Δλ )
     float x = sin(delta_longitude) * cos(tr_latitude);
     float y = cos(c_latitude) * sin(t_latitude) - sin(c_latitude) * cos(t_latitude) * cos(delta_longitude);
     float bearing = degrees(atan2(x,y));
     Serial.print("Forward azimuth: "); Serial.print(bearing); Serial.println(" degrees east or west of true north");
     
     // Turn on various LEDs or do something else fun to indicate distance to target
     if ( t_distance < 10)  {digitalWrite(13,HIGH);}
     if ( t_distance < 50)  {digitalWrite(12,HIGH);}
     if ( t_distance < 200) {digitalWrite(11,HIGH);}
      
    }
  }
}


// degrees to | from radians conversions
//float radians(float d) {return d * PI/180;} // Convert to radians from degrees
//float degrees(float r) {return r * 180/PI;} // Convert to degrees from radians

