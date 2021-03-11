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
unsigned long start_time = 0; 

// -------------------------- Block of functions:
void setup() {
 stepper1.setSpeed(-6400.0); // тут вместо setMaxSpeed() (- по идее обороты в минуту, но врет) используй setSpeed() - импульсы в секунду.
 stepper1.setAcceleration(-6400.0);
 stepper1.moveTo(-1000000);
    
 stepper2.setSpeed(6400.0);
 stepper2.setAcceleration(6400.0);
 stepper2.moveTo(1000000);
}

void loop() {

stepper1.runSpeed(); // c заданной скоростью без ускорения (по идее setAccelaration() теряяет смысл в этом случае
stepper2.runSpeed();

// тут добавляем прерывание по кнопке и внутри него присваиваем start_time = 0; 
// для UNO: INT 0 - D2, INT 1 - D3
// см. как это сделано в клипсомете:



// Период срабатывания реле 500мс:
if ((millis() - start_time) > 500){
  start_time = millis(); 
  if (pa_first.state_return() == true){
    pa_first.off(); 
    //delay(200);
  }
  else{
    pa_first.on();
    //delay(200);
  }
}


