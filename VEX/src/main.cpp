/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Authors:      Micah, Lakindu, Aidan, Gerry                              */
/*    Created:      10/28/2025, 3:53:22 PM                                    */
/*    Description:  IQ2 project — Line follow, pick cube, deliver to slot     */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"
#include "iq_cpp.h"
using namespace vex;

// Global instances
vex::brain       Brain;
vex::controller  Controller;

// MOTOR PORTS
motor motorLeft = motor(PORT9, false);   // Left drive
motor motorRight = motor(PORT10, true);  // Right drive
motor motorLift = motor(PORT7, false);   // Lift
motor clawLift = motor(PORT8, true);     // Claw

// SENSORS (add these to your robot config)
color colorSensor = color(PORT3);        // Line follower color sensor
distance distanceSensor = distance(PORT4); // Detect cube/slot box

//----------------------------------------------//
//          BASIC MOVEMENT FUNCTIONS            //
//----------------------------------------------//
void moveForward(double timeSec, double speed = 50)
{
    motorLeft.spin(forward, speed, percent);
    motorRight.spin(forward, speed, percent);
    wait(timeSec, seconds);
    motorLeft.stop();
    motorRight.stop();
}

void turnToPosition(double direction, double angle)
{
    // direction: 1 = right, 0 = left
    motorLeft.setPosition(0, degrees);
    double speed = 40;

    if (direction == 1) { // Turn right
        while (fabs(motorLeft.position(degrees)) < angle) {
            motorRight.spin(reverse, speed, percent);
            motorLeft.spin(forward, speed, percent);
        }
    } 
    else if (direction == 0) { // Turn left
        while (fabs(motorLeft.position(degrees)) < angle) {
            motorRight.spin(forward, speed, percent);
            motorLeft.spin(reverse, speed, percent);
        }
    }

    motorRight.stop();
    motorLeft.stop();
}

//----------------------------------------------//
//              ARM + CLAW CONTROL               //
//----------------------------------------------//
void lift(int direction, double position)
{
    // direction: 0 = up, 1 = down
    motorLift.setPosition(0, degrees);

    if (direction == 0) { // Up
        motorLift.spinToPosition(position, degrees, 50, percent);
    } 
    else if (direction == 1) { // Down
        motorLift.spinToPosition(-position, degrees, 50, percent);
    }
}

void claw(int direction)
{
    // direction: 1 = close, 0 = open
    if (direction == 1) {
        clawLift.spin(forward, 50, percent);
        wait(0.5, seconds);
        clawLift.stop();
    } 
    else if (direction == 0) {
        clawLift.spin(reverse, 50, percent);
        wait(0.5, seconds);
        clawLift.stop();
    }
}

//----------------------------------------------//
//           SENSOR-ACTION FUNCTIONS             //
//----------------------------------------------//
bool colourDetected(string targetColour)
{
    color hue = colorSensor.color();
    if (targetColour == "blue" && hue == color::blue) return true;
    if (targetColour == "red" && hue == color::red) return true;
    return false;
}

bool senseobject()
{
    // Returns true if something is close (< 10cm)
    return distanceSensor.objectDistance(mm) < 100;
}

//----------------------------------------------//
//            MAIN AUTONOMOUS LOGIC              //
//----------------------------------------------//
void autonomousRoutine()
{
    Brain.Screen.printAt(2, 30, "Running Autonomous Routine");

    // STEP 1: Follow line until cube is detected
    while (!senseobject()) {
        // Basic line follow — simple P logic (left/right adjust)
        if (colourDetected("blue")) {
            motorLeft.spin(forward, 30, percent);
            motorRight.spin(forward, 20, percent);
        } else {
            motorLeft.spin(forward, 20, percent);
            motorRight.spin(forward, 30, percent);
        }
        wait(20, msec);
    }

    // STEP 2: Pick up cube from left side
    motorLeft.stop();
    motorRight.stop();
    turnToPosition(0, 45);     // Turn slightly left
    claw(0);                   // Open claw
    moveForward(0.5);          // Move to cube
    claw(1);                   // Close claw to grab
    lift(0, 180);              // Lift cube up

    // STEP 3: Move to end of arena
    moveForward(3.0);          // Adjust timing to fit your arena

    // STEP 4: Turn left, go straight, turn right
    turnToPosition(0, 90);     // Left turn
    moveForward(1.0);
    turnToPosition(1, 90);     // Right turn

    // STEP 5: Check if slot has box
    if (senseobject()) {
        // If occupied, move one slot down
        moveForward(0.5);
    }

    // STEP 6: Place cube
    lift(1, 180);              // Lower lift
    claw(0);                   // Open claw to drop
    wait(1, seconds);

    // STEP 7: Return to start
    moveForward(-1.0);         // Back out
    turnToPosition(1, 180);    // Turn around
    moveForward(3.0);          // Drive back to start

    Brain.Screen.printAt(2, 60, "Routine Complete");
}

//----------------------------------------------//
//                   MAIN                       //
//----------------------------------------------//
int main() {
    Brain.Screen.printAt(2, 20, "Group 23 Robot Project - Autonomous");

    wait(2, seconds); // small delay before starting
    autonomousRoutine(); // Run the full sequence
}