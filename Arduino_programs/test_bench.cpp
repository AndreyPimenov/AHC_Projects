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
  while (millis() < (timing + delta) ){ 
    pa_first.state_return();
    };
  pa_first.state_return();
}

void setup() {
 stepper1.setSpeed(-64000.0);    
 stepper1.setAcceleration(-64000.0); 
 Serial.begin(115200);  
}

void loop() {

point = stepper1.currentPosition() - 640 ; 
stepper1.move(point);

while (stepper1.currentPosition() != point) { 
  stepper1.run();
  }
point = 0;
stepper1.setCurrentPosition (0);

delay (100);

if (pa_first.state_return() == true){
  pa_first.off(); 
  delay(50);
  
  Serial.print(pa_first.state_return());
  
  delay(50);
  Serial.println(stepper1.currentPosition());
  delay(50);
}

else{
  pa_first.on();
  delay(50);
 
  Serial.print(pa_first.state_return());
  
  delay(50);
  Serial.println(stepper1.currentPosition());
  delay(50);
  }
delay(50);

}
