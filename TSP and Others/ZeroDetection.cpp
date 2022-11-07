// Библиотеки:
#include "GyverTimers.h"
#include "GyverTimer.h"
   
// Переменные:
volatile bool flg_ap_interrupt = false;   // прерывание состоялось 
volatile bool flg_timer_allow = false;    // флаг разрешения на запуск таймера  
int potentiometr_init = 0;                // считывание с аналогового канала
int value1 = 0;  
int interval = 0;                         // интервал отсчета таймера
uint32_t debounce; 

// Функции: 
bool digitalReadFast(uint8_t pin);        // быстрый опрос пина

void setup(){
  pinMode(5, OUTPUT);
  Serial.begin(250000);                   // 9600 115200 250000
  pinMode(3, INPUT);
  attachInterrupt(1, function, CHANGE);   // LOW, FALLING - срез, RISING - фронт, CHANGE - изменение
}

bool digitalReadFast(uint8_t pin) {
  if (pin < 8) {
    return bitRead(PIND, pin);
  } else if (pin < 14) {
    return bitRead(PINB, pin - 8);
  } else if (pin < 20) {
    return bitRead(PINC, pin - 14);       // Return pin state
  }
}

void function(){ // CHANGE не предоставляет состояния пина,придется узнать его при помощи digitalReadFast
  
  if ((value1 > 51) && (value1 < 921) && (flg_ap_interrupt == false)){
    flg_timer_allow = true;      // выставляем флаг, что можно запускать таймер
    //Serial.println("2nd entering!");     // проверка на повторные срабатывания 
    value1 = analogRead(0);            // 0...1023 >>> 5% - 51, 95% - 972, возьмем 8% - 82
    interval = int (value1 * 8.7);
    flg_ap_interrupt == true;    // выставляем флаг, что прерывание состоялось 
  
}
  else {
    Serial.println("empty enters");
  }
  /*
  if ((micros() - debounce >= 20) && digitalReadFast(3) &&  !flg_ap_interrupt){
     debounce = micros();
     flg_ap_interrupt = true;             // выставляем флаг, что прерывание состоялось (по этому флагу запрещаем второй вход в прерывание)
  }
  */
}

void loop(){
  
value1 = analogRead(0);            // 0...1023 >>> 5% - 51, 95% - 972, возьмем 8% - 82
interval = int (value1 * 8.7);     // заменяем деление на 0.0087 (- цена деления) и дальнейшее округление до целого, на умножение на 8.7 мс

while(flg_timer_allow){
  // считаем время                 // запускаем пин
  // формируем меандр, мб через задержку
  // перезапускаем флаги
  
  Serial.print("interval = ");
  Serial.println(interval);
  //uint16_t interval_ms = int (interval*0.001);
  
  Serial.print("interval_ms = ");
  Serial.println(interval*0.001);
  delay(interval*0.001); // 972 деления - было 7 мс, т.е. каждое деление будет 0.0072 на 7.2 мс
  
  digitalWrite(5, HIGH);
  flg_ap_interrupt = false;
  flg_timer_allow = false; 
  delay(1); // <<--- длина меандр
  digitalWrite(5, LOW);
}

}
