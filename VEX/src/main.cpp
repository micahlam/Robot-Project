/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Authors:       Micah, Lakindu, Aidan, Gerry                             */
/*    Created:      10/28/2025, 3:53:22 PM                                    */
/*    Description:  IQ2 project                                               */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"
#include "iq_cpp.h"
using namespace vex;

// A global instance of vex::brain used for printing to the IQ2 brain screen
vex::brain       Brain;

// define your global instances of motors and other devices here

// MOTOR PORTS
// LEFT MOTOR PORT 9
motor motorLeft = motor(PORT9, false);

// RIGHT MOTOR PORT 10
motor motorRight = motor(PORT10, true);

// LIFT MOTOR 1 ON PORT 11
motor motorLift = motor(PORT11, reverse);


void turnToPosition(double direction, double angle)
{
    // 1 is right, 0 is left
    if (direction == 1){
        motorLeft.setPosition(0, degrees);
        while (abs(motorLeft.position(degrees)) < angle){
            motorRight.spin(reverse);
            motorLeft.spin(forward);}
        motorRight.stop();
        motorLeft.stop();
    }
    if (direction == 0){
        motorLeft.setPosition(0, degrees);
        while (abs(motorLeft.position(degrees)) < angle){
            motorRight.spin(forward);
            motorLeft.spin(reverse);}
    }
}

void moveForward(double time, double colour)
{
    motorLeft.spin(forward);
    motorRight.spin(forward);
}

void lift(double direction){
    if (direction == 1){
        motorLift.spin(forward);
    }
    if (direction == 0){
        motorLift.spin(reverse);
    }
}

int main() {
	
    Brain.Screen.printAt( 2, 30, "Hello IQ2" );
   
    while(1) {
        
        // Allow other tasks to run
        this_thread::sleep_for(10);
    }
}