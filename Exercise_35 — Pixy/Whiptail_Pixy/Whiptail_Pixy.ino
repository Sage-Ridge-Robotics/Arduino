/*

Sage Ridge Robotics
Whiptail-Pixy
Portions of this code are derived from the 
Adafruit Zumobot + Pixy examples

*/

// Load Communication protocol, pixy, and MotorShield libraries.  
#include <SPI.h>  
#include <Pixy.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include "servoloop.h"

// Declare tracking constants
#define X_CENTER    160L
#define Y_CENTER    100L
#define RCS_MIN_POS     0L
#define RCS_MAX_POS     1000L
#define RCS_CENTER_POS	((RCS_MAX_POS-RCS_MIN_POS)/2)
 

// Declare variables used globally in the Sketch.  
int motorPortL = 1;               
int motorPortR = 4;
int motorSpeed = 200;
int32_t size = 400;
int oldX;
int oldY; 
int oldSignature;
int scanIncrement = (RCS_MAX_POS - RCS_MIN_POS) / 150;
uint32_t lastMove = 0;
 


// Instantiate objects of the Adafruit_MotorShield,
// Adafruit_DCMotor, and Pixy classes.
Pixy pixy;

ServoLoop panLoop(200, 200);  // Servo loop for pan
ServoLoop tiltLoop(150, 200); // Servo loop for tilt

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *myDCMotorL = AFMS.getMotor(motorPortL);
Adafruit_DCMotor *myDCMotorR = AFMS.getMotor(motorPortR);

// Setup system. Start pixy computer vision subsystem. Start
// motor control board
void setup() 
{ 
  Serial.begin(9600);
  Serial.print("Starting...\n");
  pixy.init();
  AFMS.begin();
  myDCMotorL->setSpeed(motorSpeed);
  myDCMotorR->setSpeed(motorSpeed);
} 

uint32_t lastBlockTime = 0;

// Main loop
void loop() {
  uint16_t blocks;
  blocks = pixy.getBlocks();
 
  // If we have blocks in sight, track and follow them
  if (blocks){
      int trackedBlock = TrackBlock(blocks);
      FollowBlock(trackedBlock);
      lastBlockTime = millis();
  }  
  else if (millis() - lastBlockTime > 100){
    myDCMotorL->setSpeed(0);
    myDCMotorR->setSpeed(0);
    ScanForBlocks();
  }
} 

//---------------------------------------
// Track blocks via the Pixy pan/tilt mech
// (based in part on Pixy CMUcam5 pantilt example)
//---------------------------------------
int TrackBlock(int blockCount)
{
	int trackedBlock = 0;
	long maxSize = 0;
 
	Serial.print("blocks =");
	Serial.println(blockCount);
 
	for (int i = 0; i < blockCount; i++)
	{
		if ((oldSignature == 0) || (pixy.blocks[i].signature == oldSignature))
		{
			long newSize = pixy.blocks[i].height * pixy.blocks[i].width;
			if (newSize > maxSize)
			{
				trackedBlock = i;
				maxSize = newSize;
			}
		}
	}
 
	int32_t panError = X_CENTER - pixy.blocks[trackedBlock].x;
	int32_t tiltError = pixy.blocks[trackedBlock].y - Y_CENTER;
 
	panLoop.update(panError);
	tiltLoop.update(tiltError);
 
	pixy.setServos(panLoop.m_pos, tiltLoop.m_pos);
 
	oldX = pixy.blocks[trackedBlock].x;
	oldY = pixy.blocks[trackedBlock].y;
	oldSignature = pixy.blocks[trackedBlock].signature;
	return trackedBlock;
}

//---------------------------------------
// Follow blocks via the MotorShield
//
// This code makes the robot base turn 
// and move to follow the pan/tilt tracking
// of the head.
//---------------------------------------

void FollowBlock(int trackedBlock)
{
  int32_t followError = RCS_CENTER_POS - panLoop.m_pos;  // How far off-center are we looking now?
 
  // Size is the area of the object.
  // We keep a running average of the last 8.
  size += pixy.blocks[trackedBlock].width * pixy.blocks[trackedBlock].height; 
  size -= size >> 3;
 
  // Forward speed decreases as we approach the object (size is larger)
  int forwardSpeed = constrain(400 - (size/256), -150, 250);
  if (forwardSpeed < 0) {myDCMotorL->run(BACKWARD); myDCMotorR->run(BACKWARD); forwardSpeed*=-1;}

  // Steering differential is proportional to the error times the forward speed
  int32_t differential = (followError + (followError * forwardSpeed))>>8;
 
  // Adjust the left and right speeds by the steering differential.
  int leftSpeed = constrain(forwardSpeed + differential, -250, 250);
  int rightSpeed = constrain(forwardSpeed - differential, -250, 250);
 
  // And set the motor speeds
  if (leftSpeed >= 0) {myDCMotorL->setSpeed(leftSpeed); myDCMotorR->setSpeed(rightSpeed); myDCMotorL->run(FORWARD); myDCMotorR->run(FORWARD);}
  if (leftSpeed < 0) {myDCMotorL->setSpeed(leftSpeed*=-1); myDCMotorR->setSpeed(rightSpeed*=-1); myDCMotorL->run(BACKWARD); myDCMotorR->run(BACKWARD);}
}

//---------------------------------------
// Random search for blocks
//
// This code pans back and forth at random
// until a block is detected
//---------------------------------------
void ScanForBlocks()
{
	if (millis() - lastMove > 20)
	{
		lastMove = millis();
		panLoop.m_pos += scanIncrement;
		if ((panLoop.m_pos >= RCS_MAX_POS)||(panLoop.m_pos <= RCS_MIN_POS))
		{
			tiltLoop.m_pos = random(RCS_MAX_POS * 0.6, RCS_MAX_POS);
			scanIncrement = -scanIncrement;
			if (scanIncrement < 0)
			{
				myDCMotorR->setSpeed(-250);
				myDCMotorL->setSpeed(250);
			}
			else
			{
				myDCMotorL->setSpeed(+180);
				myDCMotorL->setSpeed(-180);
			}
			delay(random(250, 500));
		}
 
		pixy.setServos(panLoop.m_pos, tiltLoop.m_pos);
	}
}
