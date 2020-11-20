#include <AccelStepper.h>
#include "LED.h"
#include "Button.h"
#include "Pneumatic.h"
#include <Wire.h>
#include <LiquidCrystal_I2C_OLED.h>  // РћС‡РµРЅСЊ СЂРµРґРєР°СЏ Р±РёР±Р»РёРѕС‚РµРєР° (СЂСѓСЃРёС„РёС†РёСЂРѕРІР°РЅРЅР°СЏ)

LiquidCrystal_I2C lcd(0x27, 16, 2); // РЈСЃС‚Р°РЅР°РІР»РёРІР°РµРј РґРёСЃРїР»РµР№: Р°РґСЂРµСЃ РЅР°С‡Р°Р»СЊРЅРѕРіРѕ Р±РёС‚Р° РїРµСЂРµРґР°С‡Рё РґР°РЅРЅС‹С…, С‡РёСЃР»Рѕ СЃРёРјРІРѕР»РѕРІ РІ СЃС‚СЂРѕРєРµ, С‡РёСЃР»Рѕ СЃС‚СЂРѕРє

// main driver
#define step_main 10
#define dir_main 11

// small driver
#define step_pin 9
#define dir_pin 8

// pump driver
#define step_pump 7
#define dir_pump 6

// Create objects:
AccelStepper stepper1(AccelStepper::DRIVER, step_main, dir_main);
AccelStepper stepper2(AccelStepper::DRIVER, step_pin, dir_pin);
AccelStepper stepper3(AccelStepper::DRIVER, step_pump, dir_pump);

Led led_drive(23); // blue led
Led led_pause(22); // yellow led
Led led_alarm(25); // red led

Pneumatic pa_pusher(52);
Pneumatic pa_raker(50);
Pneumatic pa_breaker(48);
Pneumatic pa_cutter(46);
Pneumatic pa_primer(44); // <----- R E S E R V E

Button btn_drive_pos_a(34);
Button btn_drive_pos_b(35);
Button btn_cheker(37);
Button btn_breaker(36);
Button btn_raker(39);
Button btn_cutter(38);
Button btn_pusher(41);
Button btn_liner(40);
Button btn_locker(42);  // <----- R E S E R V E

// Block of the variables:
int freq = 4000;
volatile bool stop_flg = false; //
volatile bool recharge_flg = false;
volatile bool start_flg = false;
volatile uint32_t debounce;
bool alarm_flg = false;
int start_time = 0;
int act_time = 0; // Act the pump
//int cap_time = 0; // Primer cap - cap - cap - cap =)
int shift_1 = 1000; //2000
int shift_2 = 1500;   //200
//int shift_3 = 50;

// Function block:
// This is Pause_function
void Pause_function() {
  if (millis() - debounce >= 20) {
    debounce = millis();
    if (alarm_flg == false) {
      stop_flg = true;
      start_flg = false;
      recharge_flg = false;
    }
  }
}

// This is Recharge_function
void Recharge_function() {
  if (millis() - debounce >= 20) {
    debounce = millis();
    if (alarm_flg == false) {
      stop_flg = false;
      start_flg = false;
      recharge_flg = true;
    }
  }
}

void Start_function() {
  if (millis() - debounce >= 20) {
    debounce = millis();
    if (alarm_flg == false) {
      stop_flg = false;
      start_flg = true;
      recharge_flg = false;
    }
  }
}

byte Alarm_function() {
  byte code = 0b00000000;
  // Possible Alarm Situations:
  // 01 - time to moving extern the limits:
  //if ("false") { code += 0x10000; }
  // 02 - breaker moves down before start is pressed - therefore moving is not possible:
  //if ("false") { code += 0x01000; }

  // 02 - there is no KLD in the box:
  if (btn_cheker.isPressed() == false) {
    code += 0b00000100;
  }

  // 03 - liner is finished:
  if (btn_liner.isPressed() == false) {
    code += 0b00000010;
  }

  // 04 - lock is opened:
  if (btn_locker.isPressed() == false) {
    code += 0b00000001;
  }

  return code;
}


void setup() {
  pinMode(step_pin, OUTPUT);
  pinMode(dir_pin, OUTPUT);

  pinMode(step_main, OUTPUT);
  pinMode(dir_main, OUTPUT);
  digitalWrite(dir_main, HIGH);

  pinMode (step_pump, OUTPUT);
  pinMode (dir_pump, OUTPUT);
  digitalWrite(dir_pump, HIGH);

  stepper1.setMaxSpeed(6000.0);
  stepper1.setAcceleration(200.0);

  stepper2.setMaxSpeed(2000.0);
  stepper2.setAcceleration(200.0);

  stepper3.setMaxSpeed(2000.0);
  stepper3.setAcceleration(200.0);

  // Interrupt REMINDER: 0 - 2; 1 - 3; 2 - 21; 3 - 20; 4 - 19; 5 - 18; but for I2C 21 & 20 pins are used ! therefore int2 & int3 couldn't be used
  attachInterrupt(4, Start_function, RISING); // start_btn
  attachInterrupt(0, Pause_function, RISING); // cut_command
  attachInterrupt(1, Recharge_function, RISING); // reverse_command
  // attachInterrupt(5, RESERVE_FUNCTION, RISING): // <----- R E S E R V E

  // Init LCD:
  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.outStr("КЛИПСОМЕТ       ");

  // F O R _ D E B U G G I N G:
  //Serial.begin(9600);
}

void loop() {

  // C H E C K I N G _ T H E _ E N D _ E F F E C T O R S _ S Y S T E M
  /*
    Serial.print(btn_drive_pos_a.isPressed());
    Serial.print(btn_drive_pos_b.isPressed());
    Serial.print(btn_cheker.isPressed());
    Serial.print(btn_breaker.isPressed());
    Serial.print(btn_raker.isPressed());
    Serial.print(btn_cutter.isPressed());
    Serial.print(btn_pusher.isPressed());
    Serial.print(btn_liner.isPressed());
    Serial.println(btn_locker.isPressed());
    delay (500); // 1000
  */

  // C H E C K I N G _ T H E _ S T E P P E R _ S Y S T E M (just change the number of the motor exept the 1st one)
  // stepper3.setSpeed(600);
  // stepper3.runSpeed();

  // C H E C K I N G _ T H E _ P N E U M A T I C _ S Y S T E M
  /*
    pa_pusher.on();  delay (1000);    pa_pusher.off();  delay(1000);
    pa_raker.on();   delay (1000);    pa_raker.off();   delay(1000);
    pa_breaker.on(); delay (1000);    pa_breaker.off(); delay(1000);
    pa_cutter.on();  delay (1000);    pa_cutter.off();  delay(1000);
    pa_liner.on();   delay (1000);    pa_liner.off();   delay(1000);
  */

  // C O M P L E X _ C H E C K _ ( _ L C D _  & _ B T N S _)
  /*
    if (btn_drive_pos_a.isPressed()) {
    Serial.println("Pos A"); lcd.setCursor(0, 1); lcd.outStr("Положение А     "); delay (1000);
    }
    if (btn_drive_pos_b.isPressed()) {
    Serial.println("Pos B"); lcd.setCursor(0, 1); lcd.outStr("Положение Б     "); delay (1000);
    }
    if (btn_cheker.isPressed())     {
    Serial.println("Check"); lcd.setCursor(0, 1); lcd.outStr("КЛД в магазине  "); delay (1000);
    }
    if (btn_breaker.isPressed())    {
    Serial.println("Break"); lcd.setCursor(0, 1); lcd.outStr("Выбиватель дома "); delay (1000);
    }
    if (btn_raker.isPressed())      {
    Serial.println("Raker");  lcd.setCursor(0, 1); lcd.outStr("Гребенка дома   "); delay (1000);
    }
    if (btn_cutter.isPressed())     {
    Serial.println("Cuter"); lcd.setCursor(0, 1); lcd.outStr("Резак дома      "); delay(1000);
    }
    if (btn_pusher.isPressed())     {
    Serial.println("Pusher");  lcd.setCursor(0, 1); lcd.outStr("Толкатель дома  "); delay(1000);
    }
    if (btn_liner.isPressed())      {
    Serial.println("Liner"); lcd.setCursor(0, 1); lcd.outStr("Лента заправлена"); delay(1000);
    }
    if (btn_locker.isPressed())     {
    Serial.println("Locker"); lcd.setCursor(0, 1); lcd.outStr("Дверца закрыта  "); delay(1000);
    }
  */

  // T H E _ M A I N :

  // ---------------------- A L A R M ------State:
  if (alarm_flg == true) {
    led_alarm.on();
    lcd.clear();
    lcd.setCursor(0, 0); lcd.outStr(" О Ш И Б К А ");

    lcd.setCursor(0, 1); lcd.print(Alarm_function());
    delay(3000);
  }


  // ---------------------- S T A R T ------State:
  if (start_flg == true) {
    led_drive.on();
    lcd.setCursor(0, 1); lcd.outStr("   СТАРТ НАЖАТ  ");
    start_time = millis();

    while ((btn_drive_pos_b.isPressed() == 0) && (btn_breaker.isPressed() == 1)) {
      act_time = millis();
      stepper1.setSpeed(3000);
      stepper1.runSpeed();
      //stepper2.setSpeed(1200);
      //stepper2.runSpeed();

      if ((act_time - start_time) >= shift_1) {
        stepper2.setSpeed(600);
        stepper2.runSpeed();
      }

      if ((act_time - start_time) >= shift_2) {
        stepper3.setSpeed(600);
        stepper3.runSpeed();
      }
      
      if ((act_time - start_time) >= shift_3) {
        stepper3.stop();
      }

    }

    if ((btn_drive_pos_b.isPressed() == 1) /*&& (btn_breaker.isPressed() == 1) && (btn_raker.isPressed() == 1)*/) {
      //led_pause.off(); // here primer is closed;
      delay(500); pa_pusher.on(); delay(500);
      pa_breaker.on(); delay(500);
      pa_raker.on(); delay(500);

    }
    led_drive.off();
    start_flg = false;

    // Possible Alarm Situations:
    // time to moving extern the limits
    // breaker moves down therefore moving is not possible
    // there is no KLD in the box
    // liner is finished
  }


  // ---------------------- M O V E _ B A C K ------ State:

  if (recharge_flg == true) {
    led_drive.on();
    lcd.setCursor(0, 1); lcd.outStr("   НАЗАД НАЖАТ ");

    if ((btn_drive_pos_b.isPressed() == 1) && (btn_breaker.isPressed() == 1)) {
      pa_pusher.off();
      pa_raker.off();
    }
    pa_breaker.off(); delay(100); //pa_pusher.off(); delay(100); //pa_raker.off();
    while (btn_drive_pos_a.isPressed() == 0) {
      stepper1.setSpeed(-3000);
      stepper1.runSpeed();
    }
    btn_drive_pos_b.update();
    led_drive.off();
    recharge_flg = false;

    // Possible Alarm Situations:
    // time to moving extern the limits
    // breaker moves down therefore moving is not possible
    // there is no KLD in the box
    // liner is finished

  }

  // ---------------------- C U T T E R ------ State:
  if (stop_flg == true) {
    led_pause.on();
    lcd.setCursor(0, 1); lcd.outStr("   РЕЗАК НАЖАТ  ");

    pa_cutter.on(); delay(200); pa_cutter.off();  // ---------------------------------------------- CHECK IT
    delay (100);

    led_pause.off();
    stop_flg = false;
    // Possible Alarm Situations:
    // time to moving extern the limits
    // breaker moves down therefore moving is not possible
    // there is no KLD in the box
    // liner is finished
  }

}
