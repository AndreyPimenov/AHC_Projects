#include <AccelStepper.h>

#define step_main 10
#define dir_main 11
#define step_rot 6 //9c
#define dir_rot 5 //9b

AccelStepper stepper1(AccelStepper::DRIVER, step_main, dir_main);
AccelStepper stepper2(AccelStepper::DRIVER, step_rot, dir_rot);

#define PIN_INPUT_PULLUP 3

// -------------------------- Block of variables:
bool callibration_flag = false;
int angle_gofr = -128;           // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Change the angle here

// -------------------------- Block of functions:
void milsec_pause(int delta){
  long timing;        timing = millis() + delta;
  while (millis() <= timing){ 
    stepper1.runSpeed();
    };
}

void setup() {
 stepper1.setMaxSpeed(6000);
 stepper1.setSpeed(4000); // в импульсах за секунду = 200 оборот * 4 (из-за дробности шага) * 5 = 4000 импульсов за оборот
  
 stepper2.setSpeed(128000);        // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Change the speed of SMALL stepper here
 stepper2.setAcceleration(128000);    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 
 stepper2.moveTo(angle_gofr);

 Serial.begin(115200);

 pinMode(PIN_INPUT_PULLUP, INPUT_PULLUP);
}

void loop() {
 
int sensorVal = digitalRead(PIN_INPUT_PULLUP);

if (callibration_flag == false){

  // второму двигателю ехать до срабатывания концевика:
  while (sensorVal == 0){
    int sensorVal = digitalRead(PIN_INPUT_PULLUP);
    stepper2.runSpeed();

    if (sensorVal == 1){
    break;
    }
  }

  // обнулить значение текущего положения:
  stepper2.setCurrentPosition(0);
  
  // отъехать от этого положения на (0.5 + x2) шагов в другую сторону
  stepper2.moveTo(2*angle_gofr); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Change the distance here
  
  while (stepper2.distanceToGo() != 0){
  stepper2.runSpeed();
  }

  // сменить флаг
  callibration_flag = true;

 // обнулить значение текущего положения
  stepper2.setCurrentPosition (angle_gofr);
}


while(callibration_flag == true){

stepper1.runSpeed();

// change direction:
 if (stepper2.distanceToGo() == 0){
  
       // пауза milies: 
       milsec_pause(5000); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Change the delay here 
       stepper2.moveTo(-stepper2.currentPosition());
      }
 
 stepper2.runSpeed();
 }
  
}



/*#include <AccelStepper.h>

#define step_main 10
#define dir_main 11
#define step_rot 6 //9c
#define dir_rot 5 //9b

AccelStepper stepper1(AccelStepper::DRIVER, step_main, dir_main);
AccelStepper stepper2(AccelStepper::DRIVER, step_rot, dir_rot);

#define PIN_INPUT_PULLUP 3

// -------------------------- Block of variables:
bool callibration_flag = false;
int angle_gofr = -128;           // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Change the angle here
int P = 0;

// -------------------------- Block of functions:
void milsec_pause(int delta){
  long timing;        timing = millis() + delta;
  while (millis() <= timing){ 
    stepper1.runSpeed();
    //stepper1.run();
    };
}

void setup() {
 stepper1.setSpeed(4000.0);        // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Change the speed of BIG stepper here
 stepper1.setAcceleration(4000.0); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 
 stepper1.move(-500000); 
  
 stepper2.setSpeed(128000);        // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Change the speed of SMALL stepper here
 stepper2.setAcceleration(128000);    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 
 stepper2.moveTo(angle_gofr);

 Serial.begin(115200);

 pinMode(PIN_INPUT_PULLUP, INPUT_PULLUP);
}

void loop() {
 
int sensorVal = digitalRead(PIN_INPUT_PULLUP);
//P = stepper2.currentPosition();

if (callibration_flag == false){

  // второму двигателю ехать до срабатывания концевика:
  while (sensorVal == 0){
    int sensorVal = digitalRead(PIN_INPUT_PULLUP);
    stepper2.runSpeed();

    if (sensorVal == 1){
    
    break;
    }
  }

  // обнулить значение текущего положения:
  stepper2.setCurrentPosition(0);
  
  // отъехать от этого положения на (0.5 + x2) шагов в другую сторону
  stepper2.moveTo(2*angle_gofr); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Change the distance here
  
  while (stepper2.distanceToGo() != 0){
  stepper2.runSpeed();
  }

  // сменить флаг
  callibration_flag = true;

 // обнулить значение текущего положения
  stepper2.setCurrentPosition (angle_gofr);
}


while(callibration_flag == true){

// recharge varuable:
if (stepper1.distanceToGo() == 0){
    stepper1.setCurrentPosition (0);
    stepper1.moveTo(50000);
    }
    

// change direction:
 if (stepper2.distanceToGo() == 0){
  
      // пауза milies:
      // stepper2.setCurrentPosition (angle_gofr);
       
       milsec_pause(2000); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Change the delay here 
       //angle_gofr = (-1)*angle_gofr;
       stepper2.moveTo(-stepper2.currentPosition());
      }
 
 stepper2.runSpeed();
 stepper1.run();
 }
  
}
*/
