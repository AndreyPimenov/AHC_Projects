// Timer 0 Channel 1 = pin 5, Channel B = pin 6 
#include "GyverTimers.h"

// переменные, которые изменяют свое значение в прерывании - volatile
volatile int counter = 0; // счетчик прерываний
volatile bool signalStart = false;
volatile bool flag = false;
uint32_t debounce;
int frequency = 25;

bool digitalReadFast(uint8_t pin); // быстрый опрос пина

void setup(){
  pinMode(5, OUTPUT);
  Timer0.setFrequency(frequency * 2);                  // настроить частоту таймера в Гц (умножаем на 2 из-за особенностей генерации меандра
 
  Serial.begin(250000);   // 9600 115200 250000
  pinMode(3, INPUT);
  attachInterrupt(1, function, CHANGE); // LOW, FALLING - срез, RISING - фронт, CHANGE - изменение
}

bool digitalReadFast(uint8_t pin) {
  if (pin < 8) {
    return bitRead(PIND, pin);
  } else if (pin < 14) {
    return bitRead(PINB, pin - 8);
  } else if (pin < 20) {
    return bitRead(PINC, pin - 14);    // Return pin state
  }
}

void function(){ // CHANGE не предоставляет состояния пина,придется узнать его при помощи digitalReadFast
  if (micros() - debounce >= 40 && digitalReadFast(3)){
     debounce = micros();
     flag = true;
     signalStart = true;  // выставляем флаг
  }
}




void loop(){

if (signalStart == true){ 
    Timer0.start outputEnable(CHANNEL_B, HIGH);  
}

ISR(TIMER0_B) {
  digitalWrite(5, HIGH);
  signalStart = LOW;
}
  digitalWrite(5, LOW);
  

}
