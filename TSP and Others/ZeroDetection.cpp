bool digitalReadFast(uint8_t pin); // быстрый опрос пина

void setup(){
  Serial.begin(250000);   // 9600 115200 250000
  pinMode(3, INPUT);
  attachInterrupt(1, function, CHANGE); // LOW, FALLING - срез, RISING - фронт, CHANGE - изменение
}

volatile int counter = 0; // счетчик прерываний
volatile bool flag = false;
volatile uint32_t debounce;

bool digitalReadFast(uint8_t pin) {
  if (pin < 8) {
    return bitRead(PIND, pin);
  } else if (pin < 14) {
    return bitRead(PINB, pin - 8);
  } else if (pin < 20) {
    return bitRead(PINC, pin - 14);    // Return pin state
  }
}


void function(){
  // CHANGE не предоставляет состояния пина,придется узнать его при помощи digitalRead

  if (micros() - debounce >= 40 && digitalReadFast(3)){
     debounce = micros(); 
     flag = true;
     counter += 1;
     
  }
}

void loop(){

if (flag && (counter == 100) ){
  
  Serial.println("!");
  flag = false;
  counter = 0;
  }



}
