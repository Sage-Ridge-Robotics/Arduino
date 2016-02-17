/*

Sage Ridge Robotics
Example 21 

Motor function library

Modify these as required to perfect the robot.

*/


// Generates a little wiggle. Might or might not work, especially if the robot
// really needs to wiggle the other direction to get around an obstacle. 
// How could you better handle this?
void avoid_obstacle() 
{

  myDCMotor01->setSpeed(0);
  myDCMotor02->setSpeed(0); 
  myDCMotor01->run(RELEASE);
  myDCMotor02->run(RELEASE);
  
  myServo0.write(80);
  myDCMotor01->setSpeed(motorSpeed);
  myDCMotor02->setSpeed(motorSpeed); 
  myDCMotor01->run(BACKWARD);
  myDCMotor02->run(BACKWARD);
  delay(500);
  myServo0.write(90);
  myDCMotor01->run(FORWARD);
  myDCMotor02->run(FORWARD);

}

// The robot moves forward in a straight line
void move_forward() 
{
  
  myServo0.write(90);
  myDCMotor01->setSpeed(motorSpeed);
  myDCMotor02->setSpeed(motorSpeed); 
  myDCMotor01->run(FORWARD);
  myDCMotor02->run(FORWARD);
  delay(motorPeriod);

}

// The robot moves a bit to the left
void move_left_soft() {
  
  myServo0.write(80);
  myDCMotor01->setSpeed(motorSpeed);
  myDCMotor02->setSpeed(motorSpeed); 
  myDCMotor01->run(FORWARD);
  myDCMotor02->run(FORWARD);
  delay(motorPeriod);

}

// The robot moves a bit to the right
void move_right_soft() {
  
  myServo0.write(100);
  myDCMotor01->setSpeed(motorSpeed);
  myDCMotor02->setSpeed(motorSpeed); 
  myDCMotor01->run(FORWARD);
  myDCMotor02->run(FORWARD);
  delay(motorPeriod);

}

// The robot does a hard left turn
void move_left_hard() {
  
  myServo0.write(60);
  myDCMotor01->setSpeed(motorSpeed);
  myDCMotor02->setSpeed(motorSpeed); 
  myDCMotor01->run(FORWARD);
  myDCMotor02->run(FORWARD);
  delay(motorPeriod);

}

// The robot does a hard right turn
void move_right_hard() {
  
  myServo0.write(120);
  myDCMotor01->setSpeed(motorSpeed);
  myDCMotor02->setSpeed(motorSpeed); 
  myDCMotor01->run(FORWARD);
  myDCMotor02->run(FORWARD);
  delay(motorPeriod);

}

