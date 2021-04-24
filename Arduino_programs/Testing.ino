#include <AccelStepper.h>

#define step_rot 6 //9c
#define dir_rot 5 //9b
AccelStepper stepper2(AccelStepper::DRIVER, step_rot, dir_rot);

#define PIN_INPUT_PULLUP 3

// -------------------------- Block of variables:
bool mode_flag = false;
bool intial_flag = false;

int speed_direction = - 500;      // for const speed regulation
int angle_gofr = 500;             // for position regulation
int fact = 0;                     // for mesuring the error between distances
int initial = 0;                  // for measuring the initial state
int delta = 0;                    // error

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
    // delta calculation
    };
}

void stepper_distination(int angle){
  stepper2.moveTo(angle); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Change the distance here
  
  while (stepper2.distanceToGo() != 0){
    //stepper2.run(); // дало рассинхрон
    stepper2.runSpeedToPosition();
  }   
}


void setup() {
 // Управление по постоянной скорости с использованием отрезка времени дало рассинхрон значительный!
 // stepper2.setMaxSpeed(-4000);

 // Управление по положению также дало рассинхрон
 // 2й шаговый - управление по положению:
 stepper2.setSpeed(20000);
 stepper2.setAcceleration(4000);
 stepper2.move(angle_gofr);

 // Вывод для работы гофрирующей головки-качалки нужно использовать контроль по положению. но улучшить его. измеряя каждый цикл погрешность
 // Для отладки создадим переменную в которую будем записывать фактическое перемещение: fact;
 Serial.begin(115200);
 pinMode(PIN_INPUT_PULLUP, INPUT_PULLUP);
 // Испытание показало, что измеряя положение серво средствами библиотеки не дает решения проблемы, т.к. программно контроллер думает что делает
 // все верно. 
 
 // Вывод: нужен внешний контроль положения от Энкодера или хотя бы обнуление по концевикам
 
}

void loop()
{  
   
   /*
   initial = stepper2.currentPosition();
   Serial.print("Initial position: ");
   Serial.println(initial);
   
   stepper_distination(200 + delta);
   fact = stepper2.currentPosition();
   Serial.print("fact position: ");
   Serial.println(fact);

   //delta = fact - initial;
   milsec_pause(2000);
   
   stepper_distination(-200 - delta);
   milsec_pause(2000); 
   */
}




/*
#include <AccelStepper.h>

#define step_rot 6 //9c
#define dir_rot 5 //9b
AccelStepper stepper2(AccelStepper::DRIVER, step_rot, dir_rot);

#define PIN_INPUT_PULLUP 3

//-----------------------------------------------------------------
// -------------------------- Block of variables:
bool callibration_flag = false;
int angle_gofr = -500;           // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Change the angle here
int speed_direction = - 500;

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
    };
}


void setup() {
 stepper2.setMaxSpeed(-4000);
}

void loop()
{  
   
   milsec_2nd_drive(200);
   milsec_pause(2000);
   
}

// Резюме набегает ошибка за два часа сущесвтенная

// ----------------------- Далее контроль положения:
void milsec_pause(int delta){
  long timing;        timing = millis() + delta;
  while (millis() <= timing){ 
    // delta calculation
    };
}


void stepper_distination(int angle){
  stepper2.moveTo(angle); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Change the distance here
  
  while (stepper2.distanceToGo() != 0){
    stepper2.run(); //SpeedToPosition();
  }   
}


void setup() {
  // Управление по постоянной скорости с использованием отрезка времени дало рассинхрон
  //stepper2.setMaxSpeed(-4000);

 //2й шаговый - управление по положению:
 stepper2.setSpeed(20000);
 stepper2.setAcceleration(4000);
 stepper2.move(angle_gofr);
}

void loop()
{  
   
   //milsec_2nd_drive(200); // управление по времени на постоянной скорости дало рассинхрон
   
   stepper_distination(200 + delta);
   milsec_pause(2000); 
   stepper_distination(-200 - delta);
   milsec_pause(2000); 
   
}

// Резюме чистый контрол положения не спасает






