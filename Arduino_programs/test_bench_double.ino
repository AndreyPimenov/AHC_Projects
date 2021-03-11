#include <AccelStepper.h>
#include "Pneumatic.h"

#define step_top 10
#define dir_top 11
#define step_bot 6 
#define dir_bot 5 

AccelStepper stepper1(AccelStepper::DRIVER, step_top, dir_top);
AccelStepper stepper2(AccelStepper::DRIVER, step_bot, dir_bot);


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

 stepper1.setMaxSpeed(-6400.0);
 stepper1.setAcceleration(-6400.0);
 stepper1.moveTo(-1000000);
    
 stepper2.setMaxSpeed(6400.0);
 stepper2.setAcceleration(6400.0);
 stepper2.moveTo(1000000);

 
  
}

void loop() {

delay (100);

point = stepper2.currentPosition() + 140 ; // << ---- CHANGE DISTANCE HERE. where 1600 steps is half. 140 for gofr
stepper2.moveTo(point);

while (stepper2.currentPosition() != point) { 
  stepper1.run();
  stepper2.run();
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

}
