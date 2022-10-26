// Timer 0 Channel 1 = pin 5, Channel B = pin 6 
#include "GyverTimers.h"
#include "GyverTimer.h"

GTimer myTimer(US, 5000); // таймер задержки на включение
GTimer meandr(US, 5000); //  время длина меандра

// переменные, которые изменяют свое значение в прерывании - volatile
volatile bool signalStart = false;
volatile bool control_flag = false;
uint32_t debounce;
int frequency = 25;

bool digitalReadFast(uint8_t pin); // быстрый опрос пина

void setup(){
  pinMode(5, OUTPUT);
  myTimer.setInterval(5000); // 1мс
  meandr.setInterval(5000);  // 4мс
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
     signalStart = true;  // выставляем флаг
     //digitalWrite(5, HIGH);
  }
}

void loop(){


if ((signalStart == true)&&(!control_flag)){ 
    //myTimer.reset(); 
    myTimer.start();
    Serial.println("Check1");  
}

if (myTimer.isReady()) {
  control_flag = true;
  digitalWrite(5, HIGH);
  signalStart = LOW;
  //meandr.reset(); 
  meandr.start();  
  myTimer.stop();
  Serial.println("Check2"); 
}

if (meandr.isReady()) { 
  digitalWrite(5, LOW);
  meandr.stop();
  Serial.println("Check3"); 
 }

 
}
