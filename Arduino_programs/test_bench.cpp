# for AHC Revolution
#include <AccelStepper.h>
#include "Button.h"
#include "Pneumatic.h"

// Think about using liblary PinChangeInterrrupt

// main driver
#define step_main 10
#define dir_main 11

// Create objects:
AccelStepper stepper1(AccelStepper::DRIVER, step_main, dir_main);

btn start ... 
btn stop ....

pa_act1 ... 
pa_act2 .... 

// Block of the variables:
int freq = 4000;
volatile bool stop_flg = false; //
volatile bool recharge_flg = false;
volatile bool start_flg = false;
volatile uint32_t debounce;
int end_time = 0;
int act_time = 0;

void loop(){
  
  act_time = millis();
    if ((act_time - ent_time) > 2000){ 
      // ---------------------------- time delay
      stepper2.setSpeed(-1500);
      stepper2.runSpeed();
      }
  // Think how to organise sequency generator on Arduino. 
  
  // можно использовать блок который я использовал для сервомотрчика во время работы шагового двигателя
  
}


