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
int angle_gofr = -700;           // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Change the angle here
int P = 0;

// -------------------------- Block of functions:
void milsec_pause(int delta){
  long timing;        timing = millis() + delta;
  while (millis() <= timing){ 
    stepper1.runSpeed();
    stepper2.setCurrentPosition (angle_gofr);
    Serial.println("in");
    Serial.println(timing);
    if (timing == 0){
      break;
    }
    };
    timing = 0;
    Serial.println("out");
}

void setup() {
 stepper1.setSpeed(-64000.0);        // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Change the speed of BIG stepper here
 stepper1.setAcceleration(-64000.0); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 
 stepper1.moveTo(-500000); 
  
 stepper2.setMaxSpeed(8000);        // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Change the speed of SMALL stepper here
 stepper2.setAcceleration(8000);    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 
 stepper2.moveTo(angle_gofr);

 Serial.begin(115200);

 pinMode(PIN_INPUT_PULLUP, INPUT_PULLUP);
}

void loop() {
 
int sensorVal = digitalRead(PIN_INPUT_PULLUP);
P = stepper2.currentPosition();

if (callibration_flag == false){

  // второму двигателю ехать до срабатывания концевика:
  while (sensorVal == 0){
    int sensorVal = digitalRead(PIN_INPUT_PULLUP);
    stepper2.runSpeed();

    if (sensorVal == 1){
    
    break;
    }
  }

  
  
  // отъехать от этого положения на (0.5 + x2) шагов в другую сторону
  
  stepper2.moveTo(2*angle_gofr); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Change the distance here

  while (stepper2.distanceToGo() != 0){
  stepper2.runSpeed();
  }
  
  // обнулить значение текущего положения

  // сменить флаг
  callibration_flag = true;

  // обнулить значение текущего положения:
  stepper2.setCurrentPosition(0);
}


while(callibration_flag == true){
Serial.print("angle_gofr = ");
Serial.println(angle_gofr);
Serial.print("Current_position = ");
Serial.println(P);


// recharge varuable:
if (stepper1.distanceToGo() == 0){
    stepper1.setCurrentPosition (0);
    stepper1.move(50000);
    }

// change direction:
 if (stepper2.distanceToGo() == 0){
      // пауза milies:
      
      milsec_pause(500); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Change the delay here 
      angle_gofr = (-1)*angle_gofr;
      
      stepper2.move(-stepper2.currentPosition());
      }

      
 stepper2.run();
 stepper1.runSpeed();
 }

  
}
