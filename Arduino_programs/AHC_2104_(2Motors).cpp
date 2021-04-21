#include <AccelStepper.h>

#define step_main 10
#define dir_main 11
#define step_rot 6 //9c
#define dir_rot 5 //9b

AccelStepper stepper1(AccelStepper::DRIVER, step_main, dir_main);
AccelStepper stepper2(AccelStepper::DRIVER, step_rot, dir_rot);

// -------------------------- Block of variables:
int point;


// -------------------------- Block of functions:
void setup() {
 stepper1.setSpeed(-64000.0);  
 stepper1.setAcceleration(-64000.0); 
 stepper1.moveTo(50000);
  
 stepper2.setMaxSpeed(4000);
 stepper2.setAcceleration(4000);
 stepper2.moveTo(250);
}

void loop() {


if (stepper1.distanceToGo() == 0){
    stepper1.setCurrentPosition (0);
    stepper1.moveTo(50000);
  }
stepper1.run();

// Change direction:
if (stepper2.distanceToGo() == 0){
    stepper2.moveTo(-stepper2.currentPosition());
}
stepper2.run();

}

