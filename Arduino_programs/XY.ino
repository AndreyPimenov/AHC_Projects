// XY 1N3 + 5В; 3A GND;
#include <AccelStepper.h>
#include "Button.h"
#include "Pneumatic.h"

Button btn_drive_pos_Xa(3); // 1st NOC
Button btn_drive_pos_Xb(8); // 2nd NOC 

#define step_main 10 // 6A
#define dir_main 11  // 3B
#define step_rot 6   // 1N2
#define dir_rot 5    // 2B
AccelStepper stepperX(AccelStepper::DRIVER, step_main, dir_main);
AccelStepper stepperY(AccelStepper::DRIVER, step_rot, dir_rot);

// -------------------------- Block of variables:
char mode_flag = 'a';    
int bounce = 0;                                   // <<< Change the amplitude of bouncing
bool achive_position_flag = false; 

// -------------------------- Block of functions:
void milsec_pause(int delta) {
    long timing;        timing = millis() + delta;
    while (millis() <= timing) {
        //stepperX.runSpeed();
        Serial.print(btn_drive_pos_Xa.isPressed());
        Serial.print(btn_drive_pos_Xb.isPressed());
        Serial.println(mode_flag);
    };
}


void Ydrive_bounce() {
    Serial.println("Y drives bounce");
    if (stepperY.distanceToGo() == 0){
      stepperY.moveTo(-stepperY.currentPosition());
    }
    stepperY.runSpeed();
}


void Ydrive_shift(int shift) {
    stepperY.moveTo(shift);
    Serial.println("Y drives shift");
    if (stepperY.distanceToGo() == 0){
      achive_position_flag = true;
    }
    stepperY.runSpeed();
}


void setup() {
    // Управление по постоянной скорости:
    //stepperX.setMaxSpeed(-6000);
    //stepperX.setSpeed(-303);         // в импульсах за секунду = 200 оборот * 8 (из-за дробности шага) * 5 = 8000 импульсов за оборот

    // 2й шаговый - управление по положению:
    stepperY.setMaxSpeed(4000);
    stepperY.setAcceleration(2000);

    Serial.begin(115200); //for debugging
}

void loop() {
    
    /* The idea:
    mode 1:
    + X moves from A to B         << Условие входа в режим
    + Y bounce from left to right << колебание слева направо

    shift Y

    mode 2:
    - X moves from B to A
    - Y bounce form right to left

    shift Y

    Serial.print(btn_drive_pos_Xa.isPressed());
    Serial.println(btn_drive_pos_Xb.isPressed());
    delay(1000);
    */

    // The way from A to B:
    if (mode_flag == 'a') {
        stepperY.setSpeed(200);
        stepperY.move(100);
        while ((mode_flag == 'a') && (btn_drive_pos_Xa.isPressed() == 0)) { 
            stepperX.setMaxSpeed(-6000);
            stepperX.setSpeed(-303);
            stepperX.runSpeed();
            Ydrive_bounce();
            
        if (btn_drive_pos_Xa.isPressed() != 0){ break; }
        }
        
        mode_flag = 'c';
        stepperY.setCurrentPosition(0);
        //
        milsec_pause(2000);
        
    }

    // Small shift of the Y motor:
    if ((mode_flag == 'b')) {
        while ((mode_flag == 'b') && (achive_position_flag == false)){
            Ydrive_shift(-200);
            }
        mode_flag = 'c';
        stepperY.setCurrentPosition(0);
        achive_position_flag = false;
        milsec_pause(2000);
    }

    // The way from B to A:
    if (mode_flag == 'c') {
        stepperY.setSpeed(200);
        stepperY.moveTo(100);
        while ((mode_flag == 'с') && (btn_drive_pos_Xb.isPressed() == 0)) { 
             Serial.print("I'm here");
             /*
             stepperX.setMaxSpeed(-6000);
             stepperX.setSpeed(-303);
             stepperX.runSpeed();
             Ydrive_bounce();*/

       // if (btn_drive_pos_Xb.isPressed() != 0){ break; }
        }

        mode_flag = 'a';
        stepperY.setCurrentPosition(0);
        //
        milsec_pause(2000);
    }

    // Small shift of the Y motor:
    if ((mode_flag == 'd')) {
        achive_position_flag == false;
        while (achive_position_flag == false){
               Ydrive_shift(-200);
          }

        mode_flag = 'a';
        stepperY.setCurrentPosition(0);
        achive_position_flag = false;
        milsec_pause(2000);
    }
    
}
