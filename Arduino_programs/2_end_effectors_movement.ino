#include <AccelStepper.h>
#include "Button.h"
#include "Pneumatic.h"

Button btn_drive_pos_a(3); // 1st NOC
Button btn_drive_pos_b(8); // 2nd NOC 

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
        btn_drive_pos_a.isPressed();
        btn_drive_pos_b.isPressed();
    };
}

void stepper_distination(int angle, int final_angle, int st2_speed, bool znak) {
      /*
      if (znak == 1){ // едем туда
      
      // Объявляем параметры движения:
      stepper2.moveTo(angle); 
      stepper2.setMaxSpeed(1.2 * st2_speed);
      stepper2.setSpeed(st2_speed);
      stepper2.setAcceleration(st2_speed);

      return_flag = btn_drive_pos_a.isPressed();
      
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

      return_flag = btn_drive_pos_b.isPressed();
      }
    
      
   while (( return_flag == 0)&& (stepper2.distanceToGo() != 0)) {
        stepper2.setSpeed(st2_speed); 
        stepper2.runSpeed();//ToPosition();
        stepper1.runSpeed();
        if (return_flag != 0){ break; }
    }
    
    stepper2.setCurrentPosition(final_angle); // could be corrected by encoder
    */
    
}

void turn_left(int angle, int final_angle, int st2_speed){
    // Объявляем параметры движения:
      stepper2.moveTo(angle); 
      stepper2.setMaxSpeed(1.2 * st2_speed);
      stepper2.setSpeed(st2_speed);
      stepper2.setAcceleration(st2_speed);
  
    while ((stepper2.distanceToGo() != 0) && (btn_drive_pos_b.isPressed() == 0)){
    
        stepper2.runSpeed(); // дало рассинхрон, но скорость хорошая
        stepper1.runSpeed();
    if (btn_drive_pos_b.isPressed() != 0){ break; }
  }   
stepper2.setCurrentPosition(final_angle);
}

void turn_right(int angle, int final_angle, int st2_speed){
    // Объявляем параметры движения:
      stepper2.moveTo(angle); 
      stepper2.setMaxSpeed(1.2 * st2_speed);
      stepper2.setSpeed(st2_speed);
      stepper2.setAcceleration(st2_speed);
  
    while ((stepper2.distanceToGo() != 0) && (btn_drive_pos_a.isPressed() == 0)){
    
        stepper2.runSpeed(); // дало рассинхрон, но скорость хорошая
        stepper1.runSpeed();
    if (btn_drive_pos_a.isPressed() != 0){ break; }
  }   
stepper2.setCurrentPosition(final_angle);
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
       
        if (mode_flag == false) {
            // второму двигателю ехать до срабатывания концевика:
            while ((btn_drive_pos_a.isPressed() == 0) && (initial_flag == false)) {
                stepper2.setSpeed(200); // 2000
                stepper2.runSpeed();
            }
          Serial.print(btn_drive_pos_a.isPressed());
          Serial.println(btn_drive_pos_b.isPressed());
          delay(100);
            // сменить флаг
            mode_flag = true;

            // обнулить значение текущего положения
            stepper2.setCurrentPosition(0);
        }

        while (mode_flag == true) {

            stepper1.runSpeed();

            //stepper_distination(20000, 0, 200, false); // turn -400 CCW <<< have to be regulating
            turn_right(20000, 0, 3000);
            Serial.println("from");
            milsec_pause(2000);
            
            //stepper_distination(20000, 0, 300, true); // turn 400 CW 
            turn_left(20000, 0, -3000);
            Serial.println("to");
            milsec_pause(2000);
        };
}

//
// x from Xa to Xb
// y bounce as in this file
