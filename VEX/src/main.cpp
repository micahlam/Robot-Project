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
vex::controller  Controller;
// define your global instances of motors and other devices here

// MOTOR PORTS
// LEFT MOTOR PORT 9
motor motorLeft = motor(PORT9, false);

// RIGHT MOTOR PORT 10
motor motorRight = motor(PORT10, true);

// LIFT MOTOR 1 ON PORT 11
motor motorLift = motor(PORT6, false);


void turnToPosition(double direction, double angle)
{
    // 1 is right, 0 is left
    if (direction == 1){
        motorLeft.setPosition(0, degrees);
        while (abs(motorLeft.position(degrees)) < angle){
            motorRight.spin(reverse, 50, percent);
            motorLeft.spin(forward, 50, percent);}
        motorRight.stop();
        motorLeft.stop();
    }
    if (direction == 0){
        motorLeft.setPosition(0, degrees);
        while (abs(motorLeft.position(degrees)) < angle){
            motorRight.spin(forward, 50, percent);
            motorLeft.spin(reverse, 50, percent);}
        motorRight.stop();
        motorLeft.stop();
    }
}

// NOT COMPLETE
void moveForward(double time, double colour)
{
    motorLeft.spin(forward);
    motorRight.spin(forward);
}

// NOT COMPLETE
void lift(int direction, double position){
    // 0 is up, 1 is down
    motorLift.setPosition(0, degrees);
    

    


    if (direction == 1)
    {
        motorLift.spin(forward);
        motorLift.spinToPosition(position, degrees);
    }
    if (direction == 0)
    {
        motorLift.spin(reverse);
        motorLift.spinToPosition(position, degrees);
    }
}

int main() {
	
    Brain.Screen.printAt( 2, 30, "HAHA VEX PROJECT" );
    while(true){
        if (Controller.ButtonLUp.pressing()) {
            lift(0,180);
        }
        if (Controller.ButtonLDown.pressing()){
            lift(1,0);
        }}

}