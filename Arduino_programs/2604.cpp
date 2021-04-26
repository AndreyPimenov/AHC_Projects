#include <AccelStepper.h>

#define step_main 10
#define dir_main 11
#define step_rot 6 //9c
#define dir_rot 5 //9b
AccelStepper stepper1(AccelStepper::DRIVER, step_main, dir_main);
AccelStepper stepper2(AccelStepper::DRIVER, step_rot, dir_rot);

#define PIN_INPUT_PULLUP 3

// -------------------------- Block of variables:
bool mode_flag = false;
bool initial_flag = false;

int speed_direction = - 500;      // for const speed regulation
int angle_gofr = 500;             // for position regulation

// -------------------------- Block of functions:
void milsec_2nd_drive(int delta){
  long timing;        timing = millis() + delta;
  speed_direction = -1* speed_direction;
  stepper2.setSpeed(speed_direction);
  while (millis() <= timing){ 
    stepper2.runSpeed();
    };
}

void milsec_pause(int delta){
  long timing;        timing = millis() + delta;
  while (millis() <= timing){ 
    stepper1.runSpeed();
    };
}

void stepper_distination(int angle, int sensor_data, int final_angle ){
  stepper2.move(angle); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Change the distance here
  
  while ((stepper2.distanceToGo() != 0) && (sensor_data != 1)){
    int sensorVal = digitalRead(PIN_INPUT_PULLUP);
    stepper2.run(); // дало рассинхрон, но скорость хорошая
    stepper1.runSpeed();
    //stepper2.runSpeedToPosition(); // млишком медленно и тоже рассинхрон
  }   

  stepper2.setCurrentPosition (final_angle); 
}


void setup() {
 // Управление по постоянной скорости с использованием отрезка времени дало рассинхрон значительный!
 stepper1.setMaxSpeed(-6000);
 stepper1.setSpeed(-303);         // в импульсах за секунду = 200 оборот * 8 (из-за дробности шага) * 5 = 4000 импульсов за оборот

 // Управление по положению также дало рассинхрон
 // 2й шаговый - управление по положению:
 stepper2.setSpeed(-40000);
 stepper2.setAcceleration(-40000);
 stepper2.moveTo(angle_gofr);

 Serial.begin(115200);
 pinMode(PIN_INPUT_PULLUP, INPUT_PULLUP);

 
}

void loop()
{  
  
   int sensorVal = digitalRead(PIN_INPUT_PULLUP);
   if (mode_flag == false){

   // второму двигателю ехать до срабатывания концевика:
   while ((sensorVal == 0) && (initial_flag == false)){
         int sensorVal = digitalRead(PIN_INPUT_PULLUP);
         stepper2.runSpeedToPosition(); // точное и быстрое перемещение
         if (sensorVal == 1){
              initial_flag = true;
              break;
         }
  }

  // сменить флаг
  mode_flag = true;

  // обнулить значение текущего положения
  stepper2.setCurrentPosition (0);
}


while (mode_flag == true){

   stepper1.runSpeed();
   
   stepper_distination(-200, sensorVal, 0); // turn -350 CCW <<< have to be regulating
   milsec_pause(2000);
   
   stepper_distination(200, sensorVal, 0); // turn 350 CW 
   milsec_pause(2000); 
       
          };
          
}
