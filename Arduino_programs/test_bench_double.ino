#include <AccelStepper.h>
#include "Button.h"
#include "Pneumatic.h"

Button btn_drive_pos_a(3); // 1st NOC
Button btn_drive_pos_b(4); // 2nd NOC 

#define step_main 10
#define dir_main 11
#define step_rot 6 
#define dir_rot 5 
AccelStepper stepper1(AccelStepper::DRIVER, step_main, dir_main);
AccelStepper stepper2(AccelStepper::DRIVER, step_rot, dir_rot);

// -------------------------- Block of variables:
bool mode_flag = false;
bool initial_flag = false;
bool return_flag = false;

// -------------------------- Block of functions:
void milsec_pause(int delta) {
    long timing;        timing = millis() + delta;
    while (millis() <= timing) {
        stepper1.runSpeed();
    };
}

void stepper_distination(int angle, int final_angle, int st2_speed, bool znak) {

      if (znak == 0){
      Serial.println (znak);  
      Serial.println (angle);
      Serial.println (st2_speed);

      // Объявляем параметры движения:
      stepper2.moveTo(angle); 
      stepper2.setMaxSpeed(1.2 * st2_speed);
      stepper2.setSpeed(st2_speed);
      stepper2.setAcceleration(st2_speed);

      while ( (btn_drive_pos_a.isPressed() == 0)&& (stepper2.distanceToGo() != 0)) {
        stepper2.setSpeed(st2_speed); 
        stepper2.runSpeed();//ToPosition();
        stepper1.runSpeed();
     }
      
      
      } 
      else {
      angle = -1*angle;
      st2_speed = -1*st2_speed;
        
      Serial.println (znak);  
      Serial.println (angle);
      Serial.println (st2_speed);  
        

      // Объявляем параметры движения:
      stepper2.moveTo(angle); 
      stepper2.setMaxSpeed(1.2 * st2_speed);
      stepper2.setSpeed(st2_speed);
      stepper2.setAcceleration(st2_speed);
      }
    
      
   while ( (btn_drive_pos_b.isPressed() == 0)&& (stepper2.distanceToGo() != 0)) {
        stepper2.setSpeed(st2_speed); 
        stepper2.runSpeed();//ToPosition();
        stepper1.runSpeed();
    }


      
 
    
    
    stepper2.setCurrentPosition(final_angle); // could be corrected by encoder
    
}

void setup() {
     // Управление по постоянной скорости с использованием отрезка времени дало рассинхрон значительный!
        stepper1.setMaxSpeed(-6000);
        stepper1.setSpeed(-303);         // в импульсах за секунду = 200 оборот * 8 (из-за дробности шага) * 5 = 8000 импульсов за оборот

    // 2й шаговый - управление по положению:
        stepper2.setMaxSpeed(4000);
        //stepper2.setSpeed(-80000);
        stepper2.setAcceleration(2000);
        //stepper2.moveTo(angle_gofr);

        Serial.begin(115200); //for debugging
}


void loop() {
        Serial.print(btn_drive_pos_a.isPressed());
        Serial.println(btn_drive_pos_b.isPressed());
    
        /*
        if (mode_flag == false) {
            // второму двигателю ехать до срабатывания концевика:
            while ((btn_drive_pos_a.isPressed() == 0) && (initial_flag == false)) {
                stepper2.setSpeed(-200);
                stepper2.runSpeed();
                Serial.print(btn_drive_pos_a.isPressed());
                Serial.println(btn_drive_pos_b.isPressed());
            }

            // сменить флаг
            mode_flag = true;

            // обнулить значение текущего положения
            stepper2.setCurrentPosition(0);
        }

        while (mode_flag == true) {

            stepper1.runSpeed();

            
            stepper_distination(400, 0, 2000, false); // turn -400 CCW <<< have to be regulating
            Serial.println("from");
            milsec_pause(2000);
            
            
            stepper_distination(400, 0, 3000, true); // turn 400 CW 
            Serial.println("to");
            milsec_pause(2000);
            

        };*/
}

//
// x from Xa to Xb
// y bounce as in this file



/*
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


