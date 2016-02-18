/*

Sage Ridge Robotics
Example 21

Multi-sensor battle or sumo robot. 

Arduino pin assignments and system constants.

Modify as you see fit to obtimize your robot!

Copyright (c) 2016 Christopher von Nagy
This code is in the public domain.

*/

// Ultrasonic sonar
// We are using PPIN0 and VPIN0 for the left sensor and 
// PPIN1, VPIN1 for the right sensor.
// Pin assignments should only be changed if you choose 
// to use different pins.

// Digital control pins for ultrasonic sonar
#define PPIN0 2
#define PPIN1 3

// Analog voltage pins for ultrasonic sonar
#define VPIN0 0
#define VPIN1 1

// Arduino motor shield block assignments. Here the left motor is attached to block 3 and the right motor to block 4.
#define LMPORT 3 
#define RMPORT 4

// Constants. Adjust as necessary.
// NOTES:
// LUX_T should be determined experimentally to be accurate. 50 is a placeholder. Maybe it will work. maybe not.
// DIST_L depends on the size of the playing field and is really only useful when the robot detects something a full 
//   playing field diameter away.
// SEARCH_ITERATIONS and SEARCH_D_PERIOD work together. Adjust to step through a full circle. 
// BOUNDARY_CHECK_DELAY controls the interval between checks for the bounday. Too long, and the robot will roll right out
//   of the playing field
// DRIVE_ITERATIONS controls the number of boundary checks when driveForward() or driveBackward() are invoked. While cycling
//   through checks, the robot will be happily moving.
// CHECK_DELAY controls the delay between target_detect() and detect_boundary() calls when following a target. 
// LUNGE_ITERATIONS, like DRIVE_ITERATIONS, controls the number of boundary checks while the robot is lunging at a target. 
#define LUX_T 50
#define DIST_T 400
#define SEARCH_INTERATIONS 360
#define SEARCH_D_PERIOD 20
#define BOUNDARY_CHECK_DELAY 100
#define DRIVE_ITERATIONS 40
#define CHECK_DELAY 100
#define LUNGE_ITERATIONS 40


