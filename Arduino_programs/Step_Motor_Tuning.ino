#include <Keypad.h>
#include <AccelStepper.h>

AccelStepper StandDrive(1, 3, 2); // (number of motor, pin 3 = step, pin 2 = direction)

/* Техническое задание: есть несколько режимов работы двигателя:
A - режим настройки скорости (в качестве защиты от дурака вход/выход в режим и из него по долгому нажатию) 
B - режим запуска сохраненной скорости 
С - режим запуска остановка двигателя
D - режим смена направления вращения (в качестве защиты от дурака вход в режим и выход из него по долгому нажатию)

A: Некоторые особенности настройки скорости:
а) При входе в режим светодиод 13 загорелся, просигнализировав о том, что режим активен и можно снимать палец с кнопки 
б) Если ввод начался с цифр, то скорость перезаписывается
в) Если вход начался с * или # игнорирование (в последствии можно добавить функционал на эти кнопки, например * - умножить # - сложить)  
г) При выходе из режима - светодиод 13 загорелся, просигнализировав о том, что режим активен и можно снимать палец с кнопки

B: просто старт двигателя

C: просто останов двигателя

D: Режим смены направления вращения:
Переменная DIR меняет свой знак

Полезные источники:
// http://cppstudio.com/post/276 - таблица символов ASCI C 
// http://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html - обработка прерываний в Arduino по таймеру
// https://lesson.iarduino.ru/page/urok-13-podklyuchenie-klaviatury-4-4-i-pezoizluchatelya-k-arduino - пример работы с кнопкой
*/

/*Подпрограмма считывания сигналов с клавиш*/
const byte ROWS = 4; //число строк клавиатуры
const byte COLS = 4; //число столбцов клавиатуры
char hexaKeys[ROWS][COLS] = {
{'1','2','3','A'}, // здесь мы располагаем названия наших клавиш, как на клавиатуре,для удобства пользования
{'4','5','6','B'},
{'7','8','9','C'},
{'*','0','#','D'}
};

byte rowPins[ROWS] = {5, 6, 7, 8}; // выводы управления строками
byte colPins[COLS] = {9, 10, 11, 12}; // выводы управления столбцами

Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

void setup() {
  float V = 150; // Скорость (мм/с)
  float Sp = round(V * 20.1861);

  StandDrive.setMaxSpeed(2300);
  //StandDrive.setAcceleration(10.0);
  StandDrive.setSpeed(Sp);

  //pinMode (Freq_Pin, OUTPUT); //Устанавливает порт, который нам нужно переключать в ISR, выходным.
  //pinMode (DIR_Pin, OUTPUT); 
  Serial.begin(9600);
  
}

void loop(){
     
     StandDrive.runSpeed();
     //StandDrive.run();
     
     char key = customKeypad.getKey();
  if (key){
     Serial.println(key); // Передаем название нажатой клавиши в сериал порт
     delay (500);
     }
}
/*
     StandDrive.runSpeed();
     //StandDrive.run();

     char customKey = customKeypad.getKey();

     digitalWrite (DIR_Pin, HIGH);
     //digitalWrite (DIR_Pin, LOW);

switch (customKey) {
case 49: 
Freq = 0.1;
Serial.println("START button is pressed");
Serial.println (Freq);
//digitalWrite (Freq_Pin, HIGH);
tone (Freq_Pin, Freq);
break;

case 50:                        
Serial.println("Incrementing STEP+");
Freq = Freq + 0.633;
Serial.println (Freq);
tone (Freq_Pin, Freq);
break;

case 51:                       
Serial.println("Decrementing STEP-");
Freq = Freq - 0.633;
Serial.println (Freq);
tone (Freq_Pin, Freq);
break;

case 52:
Serial.println("STOP button is pressed");
Freq = Freq;
Serial.println (Freq);
noTone (6);
//digitalWrite (Freq_Pin, LOW);
break;      
  }
}

*/
