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

//--------------------------------------------------- 22.04
#include <AccelStepper.h>

#define step_main 10
#define dir_main 11
#define step_rot 6 //9c
#define dir_rot 5 //9b

AccelStepper stepper1(AccelStepper::DRIVER, step_main, dir_main);
AccelStepper stepper2(AccelStepper::DRIVER, step_rot, dir_rot);

// -------------------------- Block of variables:
bool callibration_flag = false;

// -------------------------- Block of functions:
void milsec_pause(int delta){
  int timing;        timing = millis();
  while (millis() < (timing + delta) ){ 
    };
}

void setup() {
 stepper1.setSpeed(-64000.0);  
 stepper1.setAcceleration(-64000.0); 
 stepper1.moveTo(-500000); 
  
 stepper2.setMaxSpeed(4000);
 stepper2.setAcceleration(4000);
 stepper2.moveTo(250); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<< Change the angle of the 2nd stepper here

 pinMode(4, INPUT);
}

void loop() {

if (callibration_flag == false){

  // второму двигателю ехать до срабатывания концевика:
  while (digitalRead(4) == false){
    stepper2.runSpeed();
    }

  // обнулить значение текущего положения:
  stepper2.setCurrentPosition(0);
  
  // отъехать от этого положения на 30 шагов в другую сторону
  stepper2.moveTo(-30);
  stepper2.run();
  
  // обнулить значение текущего положения
  stepper2.setCurrentPosition(0);
  
  // сменить флаг
  callibration_flag = true;
}


while(callibration_flag == true){

  stepper1.runSpeed(); // or stepper1.run();
  /*
  if (stepper1.distanceToGo() == 0){
    stepper1.setCurrentPosition (0);
    stepper1.moveTo(50000);
  }
stepper1.run();
*/

  // Change direction:
  if (stepper2.distanceToGo() == 0){

      // пауза milies:
      milsec_pause(200); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Change the delay here 
   
      stepper2.moveTo(-stepper2.currentPosition());
      }
   
  stepper2.run();
  }
}

