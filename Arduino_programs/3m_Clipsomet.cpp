#include <AccelStepper.h>
#include "LED.h"
#include "Button.h"
#include "Pneumatic.h"
#include "LCD_Screen.h"

// pump driver
#define step_pump 6
#define dir_pump 5

// small driver 
#define step_pin 9
#define dir_pin 8

// main driver
#define step_main 10
#define dir_main 11

// Create objects:
AccelStepper stepper1(AccelStepper::DRIVER, step_main, dir_main);
AccelStepper stepper2(AccelStepper::DRIVER, step_pin, dir_pin);
AccelStepper stepper3(AccelStepper::DRIVER, step_pump, dir_pump);
 
Led led_drive(22); // green led
Led led_pause(23); // yellow led
Led led_alarm(24); // red led

Pneumatic pa_primer(26);
Pneumatic pa_breaker(27);
Pneumatic pa_cutter(28);
Pneumatic pa_raker(29);
Pneumatic pa_pusher(30);

Button btn_drive_pos_a(34);
Button btn_drive_pos_b(35);
Button btn_cheker(37);
Button btn_breaker(38);
Button btn_cutter(39);
Button btn_raker(40);
Button btn_pusher(41);
Button btn_loсker(42);
Button btn_liner(43);

// Block of the variables:
int freq = 4000; //4000 - max for this driver check for other one
volatile bool stop_flg = false; // Flag of the stop button; <-- This could be started immediately!!!
volatile bool recharge_flg = false; // Flag for recharging line in the machine; <-- This could be start not immediately after preparation
volatile bool start_flg = false; //Flag to start nwew cycle;

volatile uint32_t debounce;

// Function block:

// This is Pause_function
void Pause_function() {
  if (millis() - debounce >= 20 && digitalRead(21)) { // it was 100;
    debounce = millis();
    stop_flg = true;
    start_flg = false;
    recharge_flg = false;
    
  }
}

// This is Recharge_function
void Recharge_function() {
  if (millis() - debounce >= 100 && digitalRead(3)) {
    debounce = millis();
    recharge_flg = true;
    start_flg = false;
  }
}

void Start_function() {
  if (millis() - debounce >= 100 && digitalRead(2)) {
    debounce = millis();
    start_flg = true;
    recharge_flg = false;
  }
}


void setup() {
  pinMode (step_pin, OUTPUT);
  pinMode (dir_pin, OUTPUT);

  pinMode (step_main, OUTPUT);
  pinMode (dir_main, OUTPUT);
  digitalWrite(dir_main, HIGH);

// additional drive:
  pinMode (step_pump, OUTPUT);
  pinMode (dir_pump, OUTPUT);
  digitalWrite(dir_pump, HIGH);

  stepper1.setMaxSpeed(6000.0);    // 6000
  stepper1.setAcceleration(200.0); // 200

  stepper2.setMaxSpeed(2000.0);    // 2000
  stepper2.setAcceleration(200.0); // 200

  stepper3.setMaxSpeed(2000.0);    // 2000
  stepper3.setAcceleration(200.0); // 200

 
  attachInterrupt(0, Start_function, RISING); // start_btn
  attachInterrupt(2, Pause_function, RISING); // pause_btn
  attachInterrupt(1, Recharge_function, RISING); // recharge_btn
  // Reserve:
  // attachInterrupt(3, null_func, RISING);
  // attachInterrupt(4, null_func, RISING);
  // attachInterrupt(5, null_func, RISING);

}


// Previous Version:

void loop() {
  if (start_flg == true) {
    while ((btn_drive_pos_b.isPressed() == 0)&& (btn_breaker.isPressed() == 1)) { // while this end_effector is not pressed do following:
      pa_primer.on();  pa_breaker.off(); //pa_raker.off(); pa_cutter.off(); pa_pusher.off();
      stepper1.setSpeed(3000);
      stepper1.runSpeed();
      //delay(200);
      stepper2.setSpeed(-1500);
      stepper2.runSpeed();
    }
    
    pa_primer.off();  
    
    if ((btn_drive_pos_b.isPressed() == 1) && (btn_breaker.isPressed() == 1) && (btn_raker.isPressed() == 1)){ 
    // if ((btn_drive_pos_b.isPressed() == 1)){ 
    delay(500); pa_pusher.on(); delay(500);
    pa_breaker.on(); delay(500);
    pa_raker.on(); delay(500);

    //recharge_flg = true;
  }
}

if (recharge_flg == true) {
  
  if ((btn_drive_pos_b.isPressed() == 1) && (btn_breaker.isPressed() == 1)) {
    pa_pusher.off();
    pa_raker.off();
  }
  pa_breaker.off(); delay(100); pa_pusher.off(); delay(100); pa_raker.off();
  while (btn_drive_pos_a.isPressed() == 0) {
    stepper1.setSpeed(-3000); 
    stepper1.runSpeed();
  }
}

// I add this fubction to button:
if (stop_flg == true){
  pa_cutter.on(); delay(200); pa_cutter.off();  // ---------------------------------------------- CHECK IT
  delay (100);
  stop_flg = false;
}

}
