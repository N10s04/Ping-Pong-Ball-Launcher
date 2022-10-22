#pragma region VEXcode Generated Robot Configuration
// Make sure all required headers are included.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>


#include "vex.h"

using namespace vex;

// Brain should be defined by default
brain Brain;


// START V5 MACROS
#define waitUntil(condition)                                                   \
  do {                                                                         \
    wait(5, msec);                                                             \
  } while (!(condition))

#define repeat(iterations)                                                     \
  for (int iterator = 0; iterator < iterations; iterator++)
// END V5 MACROS


// Robot configuration code.
motor armMotor = motor(PORT2, ratio18_1, false);

motor ballMotor = motor(PORT1, ratio18_1, false);

pot angleSelector = pot(Brain.ThreeWirePort.F);
bumper angleConfirm = bumper(Brain.ThreeWirePort.G);
limit limitSwitch = limit(Brain.ThreeWirePort.H);
#pragma endregion VEXcode Generated Robot Configuration

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Nicholas D Thoennes                                       */
/*    Created:      1/19/22                                                   */
/*    Description:  Projectile Launcher Project                               */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// Include the V5 Library
#include "vex.h"
  
// Allows for easier use of the VEX Library
using namespace vex;

/*everything above here is vex stuff, don't touch it*/


/* 
  ***Stuff To Do***
  - support the arm better
  - figure out gear ratios for faster wheels -- keep pulleys consistent diameter so we can ignore them (Dout/Din=Omegain/Omegaout)
  - measure minimum angle from vertical and adjust from there in the code so that it displays the correct angle on the screen when you hit launch
  - display the range on the screen
  - sketch four brainstorming ideas
  - sketch the final design and explain why we picked it
  - document the modifications, explain the issue and how we fixed it
  - take pictures
  - orthographic drawings and pictures
  - draw schematics
  - document code
  - show calculations
  - fill in data tables
  - reflection questions
*/


//variables
double theta = 0; //the armMotor angle in degrees
int ballVelocity = 600; //ballMotor velocity in rpm ... 
                        //rpm * 2pi/60 * wheelRadius in m = velocity in m/s
double wheelRadius = 0.35;//meters
double ballVelocitySI = ((ballVelocity*2*3.14)/60)*wheelRadius;

//launch the ball
void launch() {
  //spin the ball motor launcher arm thing really fast for fifteen seconds
  ballMotor.setVelocity(ballVelocity, rpm);
  ballMotor.spin(reverse);
  wait(10, seconds);
  ballMotor.stop();
}

//kinemagics
void kinemagics() {
  //user turns potentiometor to set range in cm
  //presses a bumper to confirm the range
  while(true){
    double potAngle = angleSelector.angle(degrees);
    wait(0.0005, seconds);
    armMotor.spinToPosition((potAngle*4), degrees); //move the arm to that position

    if (angleConfirm.pressing()){ //if the button is pressed, stop, print the range, and launch the balls
      //kinematics range equation R = (Vinitial^2sin(2theta))/g
      double range = (pow(ballVelocitySI, 2) * sin(2*potAngle)) / 9.8;
      Brain.Screen.clearScreen();
      Brain.Screen.setCursor(2,1);
      Brain.Screen.print(range);
      launch();
      break;
    }
  }
}

//home the arm - this is the first thing that happens
void homing() {
 armMotor.setVelocity(10, percent);
 //negative direction is up
 armMotor.spin(reverse);
 //keep moving until the limit switch is pressed
 while(true){
   if(limitSwitch.pressing()){
     //stop the motor
     armMotor.stop();
     //set the reference position (0)
     armMotor.setPosition(0, degrees);
     //we done homing :)
     break;
   }
 }
}

int main() {
  homing(); //home the arm
  while(true){
    //reset motor position
    //call kinemagics
    kinemagics();
  }
}
