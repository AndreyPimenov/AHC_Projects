// Библиотеки:
#include "GyverTimers.h"
#include "GyverTimer.h"
   
// Переменные:
volatile bool flg_ap_interrupt = false;   // прерывание состоялось 
volatile bool flg_timer_allow = false;    // флаг разрешения на запуск таймера  
int potentiometr_init = 0;                // считывание с аналогового канала
long value1 = 0;  
long interval = 0;                         // интервал отсчета таймера
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
  
  if ((value1 > 51) && (value1 < 972) && (flg_ap_interrupt == false)){
    flg_timer_allow = true;      // выставляем флаг, что можно запускать таймер
    Serial.println("2nd entering!");     // проверка на повторные срабатывания 
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
  
value1 = analogRead(0);          // 0...1023 >>> 5% - 51, 95% - 972
interval = value1 * 115;     // заменяем деление с точкой / 0.0087 и округление на умножение на целое

while(flg_timer_allow){
  Serial.print("interval = ");
  Serial.println(interval);
  uint16_t interval_ms = int (interval*0.001);
  Serial.print("interval_ms = ");
  Serial.println(interval_ms);
  delay(interval_ms); // 8мс - 972 ............................1 мс - 51 
  
  digitalWrite(5, HIGH);
  flg_ap_interrupt = false;
  flg_timer_allow = false; 
  delay(10); //10 ms
  // считаем время
  // запускаем пин
  // формируем меандр, мб через задержку
  // перезапускаем флаги
  digitalWrite(5, LOW);
  
}


}
