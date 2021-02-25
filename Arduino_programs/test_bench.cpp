#include <AccelStepper.h>
#include "Pneumatic.h"

// main driver
#define step_main 10
#define dir_main 11

// Create objects:
AccelStepper stepper1(AccelStepper::DRIVER, step_main, dir_main);

Pneumatic pa_first(8);
Pneumatic pa_second(7); // << --------- RESERVE

// Block of variables:
int point = 0;
bool state_flag = 0;
unsigned long start_time; 

// Block of functions:
void milsec_pause(int delta){
  int timing;
  timing = millis();
  while (millis() < (timing + delta) ){  
  
 };
  }

void setup() {

  pinMode(step_main, OUTPUT);
  pinMode(dir_main, OUTPUT);
  digitalWrite(dir_main, HIGH);

  stepper1.setMaxSpeed(6400.0);    
  stepper1.setAcceleration(6400.0); 
  
  // F O R _ D E B U G G I N G:
  // Serial.begin(9600);
}

void loop() {

// C H E C K I N G _ T H E _ S T E P P E R _ S Y S T E M 
//stepper1.setSpeed(600);
//stepper1.runSpeed();

point = stepper1.currentPosition() + 140 ; // << ---- CHANGE DISTANCE HERE. where 1600 steps is half. 140 for gofr
stepper1.moveTo(point);

while (stepper1.distanceToGo() != 0){
  stepper1.run();
  }

//milsec_pause(100); 


// C H E C K I N G _ T H E _ P N E U M A T I C _ S Y S T E M 
//if (stepper1.currentPosition() == point){

  if (state_flag == 1){
     pa_first.on();  //delay (1000);
     //start_time = millis()
     milsec_pause(300); 
    }
  else if (state_flag == 0){ 
    pa_first.off();  //delay (1000); 
    milsec_pause(300);
    }
  state_flag = !state_flag; 
  //}
  
}
