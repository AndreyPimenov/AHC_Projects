#include <AccelStepper.h>
#include "Pneumatic.h"

// main driver
#define step_main 10
#define dir_main 11

// Create objects:
AccelStepper stepper1(AccelStepper::DRIVER, step_main, dir_main);

Pneumatic pa_first(8);
Pneumatic pa_second(7); // << --------- RESERVE

// Block of the variables:
int point = 0;

void setup() {

  pinMode(step_main, OUTPUT);
  pinMode(dir_main, OUTPUT);
  digitalWrite(dir_main, HIGH);

  stepper1.setMaxSpeed(6000.0);    //
  stepper1.setAcceleration(2000.0); //
  
  // F O R _ D E B U G G I N G:
  // Serial.begin(9600);
}

void loop() {

// C H E C K I N G _ T H E _ S T E P P E R _ S Y S T E M 
//stepper1.setSpeed(600);
//stepper1.runSpeed();

point = stepper1.currentPosition() + 1600 ; // << ---- CHANGE DISTANCE HERE. where 1000 steps
stepper1.moveTo(point);

while (stepper1.distanceToGo() != 0){
  stepper1.run();
  }

delay(500); // << --- it saves from unlimited movement

// C H E C K I N G _ T H E _ P N E U M A T I C _ S Y S T E M _
if (stepper1.currentPosition() == point){
  pa_first.on();  delay (1000);    pa_first.off(); delay(1000);
  }
}
