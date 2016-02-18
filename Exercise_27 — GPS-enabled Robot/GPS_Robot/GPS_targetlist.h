/*

Sage Ridge Robotics
Example 27

GPS-enabled robot

Navigation goals array
Targets are in decimal degrees. 
Latitiude is navGoal[goal][0]; Longitude navGoal[goal][1]
The number of goals must match the number of rows in the 
navGoals array.

You might use the logging ability of a GPS device
or GoogleEarth to develop a list of GPS targets for the
GPS robot exercise.

*/

// This set of targets runs along the back road at Sage Ridge.
// Try getting your robot to autonomously navigate the entire way 
// around the school!

const int numGoals = 6;
float navGoals[numGoals][2] =
 {
    {39.407543, -119.799252}, 
    {39.407888, -119.799872},
    {39.407983, -119.799998},
    {39.408112, -119.800130},
    {39.408384, -119.800179},
    {39.409531, -119.800293}
};



