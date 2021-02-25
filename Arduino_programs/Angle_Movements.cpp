// пины для подключения контактов STEP, DIR
#define PIN_STEP 3
#define PIN_DIR 2

// количество шагов на 1 оборот
#define STEP_ROUND 200

// скорость двигателя
#define SPEED 10

// Пригодиться в пунтке 5 твоего алгоритма
int drive_position = 84; //2*84*0.9 /1.8 = 84

void rotation_function(int STEP, bool DIR, int TIME){
// направление вращения
digitalWrite(PIN_DIR, DIR); // DIR == TRUE - HIGH; DIR == FALSE - LOW 
// сделать 1 оборот
for(int j = 0; j < STEP; j++) {
digitalWrite(PIN_STEP, HIGH);
delay(SPEED);
digitalWrite(PIN_STEP, LOW);
delay(SPEED);
}
delay (TIME);

}


void setup() {
// режим для STEP и DIR как OUTPUT
pinMode(PIN_STEP, OUTPUT);
pinMode(PIN_DIR, OUTPUT);
// начальные значения
digitalWrite(PIN_STEP, 1);
digitalWrite(PIN_DIR, 0);
}

void loop() {
delay(5000);
rotation_function(171,0,3000); //пункт 1 твоего алгоритма 342*0.9 / 1.8 = 171 шага,потому что полношаговый режим 190
rotation_function(171,1,3000); //пункт 2 твоего алгоритма 

for (int i = 1; i < 14; i++){ //вход в цикл

delay (3000);
rotation_function(drive_position,0,3000); //пункт 3 твоего алгоритма 
rotation_function(drive_position/2,1,3000);  //пункт 4 твоего алгоритма 
rotation_function(drive_position,0,3000); //пункт 5 твоего алгоритма 
rotation_function(drive_position*3/2,1,250); //пункт 6 твоего алгоритма 
rotation_function(3,0,250); // смещение 3 шага
}
delay (10000);
}
