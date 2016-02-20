/*

Sage Ridge Robotics
Example 27 

Motor function library

Modify these as required to perfect your robot.

*/

void driveForward () {
  Serial.println("Driving ... "); 
  myLeftDCMotor->setSpeed(250); 
  myRightDCMotor->setSpeed(250);
  myLeftDCMotor->run(FORWARD); 
  myRightDCMotor->run(FORWARD); 
  delay(motorInterval);
}

void rotate_to_f_azimuth (float a_correction) {
  
  int iterations = floor(abs(a_correction));
  int d_period = 20;
  myLeftDCMotor->setSpeed(100); 
  myRightDCMotor->setSpeed(100);
  
  if ( a_correction < 0 ) {
    Serial.print("Correcting left "); 
    Serial.print(iterations);
    Serial.print(" degrees ");
    for (int i=0; i < iterations; i++) {
      Serial.print(".");
      myLeftDCMotor->run(BACKWARD); 
      myRightDCMotor->run(FORWARD);
      delay(d_period);
    }
    myLeftDCMotor->run(RELEASE); 
    myRightDCMotor->run(RELEASE);
    Serial.println(" done"); 
  } 
  
  if ( a_correction > 0 ) {
    Serial.print("Correcting right "); 
    Serial.print(iterations);
    Serial.print(" degrees ");
    for (int i=0; i < iterations; i++) {     
      Serial.print(".");
      myLeftDCMotor->run(FORWARD); 
      myRightDCMotor->run(BACKWARD);
      delay(d_period);
    }
    myLeftDCMotor->run(RELEASE); 
    myRightDCMotor->run(RELEASE);
    Serial.println(" done"); 
  }   
}

void full_stop() {
       myLeftDCMotor->run(RELEASE); 
      myRightDCMotor->run(RELEASE);
}
