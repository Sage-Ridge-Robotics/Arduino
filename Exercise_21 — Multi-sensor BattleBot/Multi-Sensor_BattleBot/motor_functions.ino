/*

Sage Ridge Robotics
Example 21

Multi-sensor battle or sumo robot. 

Motor functions.

Modify as you see fit to obtimize your robot!

Copyright (c) 2016 Christopher von Nagy
This code is in the public domain.

Speed settings may vary from 0 to 255.
Directions may be FORWARD, BACKWARD, and RELEASE (stop).


*/

// Start the motor shield
boolean start_motor_shield() {
  
  Serial.print("Starting motors . . . ");
  AFMS.begin();
  Serial.println("good to go!");
 
  return 1;
 
  // Not at all necessary. Function here just to demonstrate boolean and return.  

}

// What to do when driveForward() is called.
void driveForward() {
  
  Serial.println("Driving ... "); 
  leftMotor->setSpeed(100); 
  rightMotor->setSpeed(100);
  leftMotor->run(FORWARD);
  rightMotor->run(FORWARD);
  for (int i = 0; i > DRIVE_ITERATIONS; i++ ) { delay(BOUNDARY_CHECK_DELAY); if ( detect_boundary() ) { full_stop(); } }
  
}

// What to do when driveBackward() is called.
void driveBackward() {
  
  Serial.println("Full reverse ... "); 
  leftMotor->setSpeed(150); 
  rightMotor->setSpeed(150);
  leftMotor->run(FORWARD);
  rightMotor->run(FORWARD); 
  for ( int i = 0; i > DRIVE_ITERATIONS; i++ ) { delay(BOUNDARY_CHECK_DELAY); if ( detect_boundary() ) { full_stop(); } }
  
}


// What to do when the search function is called.
// Returns true when target is acquired.

// iterations sets the number of increments to turn a full circle.
// d_period sets the length of time to run the motors.
// Adjust these to move through one full circle in a set number of increments.
// You might try five degree rather than 1 degree increments
// iterations = 72.

int search(int sD) 
{
  
  // SEARCH_INTERATIONS and SEARCH_D_PERIOD are defined in pin_layout.h
  
  leftMotor->setSpeed(50); 
  rightMotor->setSpeed(50);
  
  if (sD = 1) {
    Serial.print("Searching clockwise "); 
    for (int i = 0; i < SEARCH_INTERATIONS; i++ ) {     
      Serial.print(".");
      target_detect();
      if ( target_detected ) { Serial.println(" target detected!"); break;}
      leftMotor->run(FORWARD); 
      rightMotor->run(BACKWARD);
      delay(SEARCH_D_PERIOD);    
    }
    leftMotor->run(RELEASE); 
    rightMotor->run(RELEASE);
  }    

  if (sD = 0) {
    Serial.print("Searching anti-clockwise "); 
    for (int i=0; i < SEARCH_INTERATIONS; i++ ) {     
      Serial.print(".");
      target_detect();
      if ( target_detected ) { Serial.println(" target detected!"); break;}
      leftMotor->run(BACKWARD); 
      rightMotor->run(FORWARD);
      delay(SEARCH_D_PERIOD);
    }
    leftMotor->run(RELEASE); 
    rightMotor->run(RELEASE);
  }    

  if ( ! target_detected ) { Serial.println(" no target detected."); }
  
}

// What to do when a target is to be followed. Set speed to 150. Move forward. Loop while target is detected
// adjusting direction toward the target. If a boundary is detected react. If the target is lost, go back to 
// search.
void follow_target() 
{
  
  Serial.println("Following ... "); 

  leftMotor->setSpeed(150);
  rightMotor->setSpeed(150);
  
  leftMotor->run(FORWARD); 
  rightMotor->run(FORWARD);

  while ( target_detected ) { 
    if ( detect_boundary() ) { full_stop(); Serial.println("Reached boundary"); driveBackward(); break; }
    else if ( ! target_proximate && target_position == -1) {leftMotor->setSpeed(75); rightMotor->setSpeed(150); } 
    else if ( ! target_proximate && target_position ==  0) {leftMotor->setSpeed(150); rightMotor->setSpeed(150); } 
    else if ( ! target_proximate && target_position ==  1) {leftMotor->setSpeed(150); rightMotor->setSpeed(75); }
    else if ( target_proximate ) { break; }
    delay(CHECK_DELAY);
    target_detect(); 
  }
  
  // Check for target code. Adjust. Loop
  
}

// What to do when a target is within striking distance.
void lunge() 
{
  
  Serial.println("Lunging ... "); 
  leftMotor->setSpeed(250); 
  rightMotor->setSpeed(250);
  leftMotor->run(FORWARD); 
  rightMotor->run(FORWARD);
  for ( int i = 0; i > LUNGE_ITERATIONS; i++ ) { delay(BOUNDARY_CHECK_DELAY); if ( detect_boundary() ) { full_stop(); } }
  
}

// What to do when full_stop() is called
void full_stop() {
  
  leftMotor->run(RELEASE); 
  rightMotor->run(RELEASE);
      
}

