#include <AccelStepper.h>
#include "Pneumatic.h"

#define step_main 10
#define dir_main 11

AccelStepper stepper1(AccelStepper::DRIVER, step_main, dir_main);

Pneumatic pa_first(8);

// -------------------------- Block of variables:
int point = 0;
bool state_flag = 0;
unsigned long start_time; 

// -------------------------- Block of functions:
void milsec_pause(int delta){
  int timing;
  timing = millis();
  while (millis() < (timing + delta) ){  };
}

void rotation_function(int steps, bool dir_flag, int half_frequency, int time_delay){
/* 
 *  steps - кол-во шагов, например для 16-кратного мы использовали 140 импульсов
 *  dir_flag - отвечает за направление вращения (меняй: 1 или 0 как тебе нужно)
 *  Half_frequency - несколько милисекунд на то чтобы физически двигатель повернулся, думаю хватит значений 1 - 5 (милисекунд) 
 *  time_delay - задает паузу после перемещения, думаю от 100 до 300 более чем достаточно
 *  Базовое значение функции: rotation_function(140, 1, 2, 100);

*/
digitalWrite(dir_main, dir_flag); 

for(int j = 0; j < steps; j++) {
digitalWrite(step_main, HIGH);
milsec_pause(half_frequency);                 
digitalWrite(step_main, LOW);
milsec_pause(half_frequency);
}

milsec_pause(time_delay);      
}


void setup() {

  pinMode(step_main, OUTPUT);
  pinMode(dir_main, OUTPUT);
  digitalWrite(dir_main, HIGH);

  pinMode(7, OUTPUT);

 stepper1.setMaxSpeed(6400.0);    
 stepper1.setAcceleration(6400.0); 
  
}

void loop() {

/*
point = stepper1.currentPosition() + 140 ; // << ---- CHANGE DISTANCE HERE. where 1600 steps is half. 140 for gofr
stepper1.moveTo(point);

while (stepper1.distanceToGo() != 0){
  stepper1.run();
  }

  if (state_flag == 1){
     pa_first.on();  
     //start_time = millis()
     milsec_pause(300); 
    }
  else if (state_flag == 0){ 
    pa_first.off();   
    milsec_pause(300);
    }
  state_flag = !state_flag; 
*/
// ВОТ И ВЕСЬ КОД:

delay (100);
//rotation_function(160, 1, 2, 200); //140
point = stepper1.currentPosition() + 140 ; // << ---- CHANGE DISTANCE HERE. where 1600 steps is half. 140 for gofr
stepper1.moveTo(point);

while (stepper1.currentPosition() != point) { 
  stepper1.run();
  }

delay (100);


if (pa_first.state_return() == true){
  pa_first.off(); 
  delay(500);
}

else{
  pa_first.on();
  delay(500);
  }
delay(200);

//pa_first.on();
//milsec_pause(200);
//delay (200);

//rotation_function(140, 1, 2, 300);
//delay(100);

//pa_first.off(); 
//milsec_pause(200);
//delay(200);
}
