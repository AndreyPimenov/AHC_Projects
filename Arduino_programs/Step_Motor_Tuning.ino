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
// https://soltau.ru/index.php/arduino/item/373-kak-vypolnyat-parallelnye-zadachi-threads-v-programme-dlya-arduino - многопоточность
// https://arduinomaster.ru/program/massivy-arduino/ - работа с массива в Ардуино
// https://arduinomaster.ru/program/arduino-string-stroki/ - работа со строками в Ардуино

// 49 = 1
// 50 = 2
// 51 = 3
// 52 = 4
// 53 = 5
// 54 = 6
// 55 = 7
// 56 = 8
// 57 = 9

Нужна функция, которая считывает изменения в клавиатуре и выдает два параметра:
а) - изменения произошли
б) - возвращает Key который был нажат 

*/

#include <Keypad.h>
#include <AccelStepper.h>
#include <Thread.h>

// Блок переменных по управлению двигателем:
AccelStepper StandDrive(1, 3, 2); // (motor number, pin 3 = step, pin 2 = direction)
float V; // Скорость 150 по умолчанию (мм/с)
float Sp = round(V * 20.1861); 

// Блок задания параметров клавиатуры:
const byte ROWS = 4; 
const byte COLS = 4; 
char hexaKeys[ROWS][COLS] = {
{'1','2','3','A'}, 
{'4','5','6','B'},
{'7','8','9','C'},
{'*','0','#','D'}
};
byte rowPins[ROWS] = {4, 5, 6, 7}; 
byte colPins[COLS] = {8, 9, 10, 11}; 
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// Блок общих переменных:
int click_ind = 0; // подсчет разрядности введенного числа
int current_index = 0;
int previous_index = 0;

int digit = 0; // цифра для записи в разряд 
int number = 0; // цифра скорости

bool driver_flag = false;
bool permission = false; 
// pin 12  -  резервный 

// Подпрограмма считывания сигналов с клавиш:
int buttoncheck(Keypad keypad_N, char key_local){
/*
Функция, которая считывает изменения в клавиатуре и возвращает Key который был нажат 
IDLE - не активирована
PRESSED - нажата
HOLD - удерживается
RELEASED - отпущена
*/
  previous_index = click_ind;
  switch (keypad_N.getState()){

    case PRESSED:
      switch (key_local){
        case '0': digit = 0; Serial.println("P digit0"); click_ind = click_ind +1; current_index = click_ind; delay(200); break;
        case '1': digit = 1; Serial.println("P digit1"); click_ind = click_ind +1; current_index = click_ind; delay(200); break;
        case '2': digit = 2; Serial.println("P digit2"); click_ind = click_ind +1; current_index = click_ind; delay(200); break;
        case '3': digit = 3; Serial.println("P digit3"); click_ind = click_ind +1; current_index = click_ind; delay(200); break;  
        case '4': digit = 4; Serial.println("P digit4"); click_ind = click_ind +1; current_index = click_ind; delay(200); break;
        case '5': digit = 5; Serial.println("P digit5"); click_ind = click_ind +1; current_index = click_ind; delay(200); break;
        case '6': digit = 6; Serial.println("P digit6"); click_ind = click_ind +1; current_index = click_ind; delay(200); break;
        case '7': digit = 7; Serial.println("P digit7"); click_ind = click_ind +1; current_index = click_ind; delay(200); break;
        case '8': digit = 8; Serial.println("P digit8"); click_ind = click_ind +1; current_index = click_ind; delay(200); break;
        case '9': digit = 9; Serial.println("P digit9"); click_ind = click_ind +1; current_index = click_ind; delay(200); break;
        case 'A': Serial.println("A button PRESSED");     click_ind = 0; previous_index = 0; current_index = 0; number = 0; delay(200); break;
        //case 'B': Serial.println("B button PRESSED"); delay(200); break;
        //case 'C': Serial.println("C button PRESSED"); delay(200); break;
        //case 'D': Serial.println("D button PRESSED"); delay(200); break;
        case '*': permission = true; break;
        case '#': click_ind = 0; current_index = 0; previous_index = 0; digit = 0; number = 0; break;
    }

   if (current_index != previous_index) {
       Serial.println("There is a moment");
       number = number*10 + digit;    
   }
   if (number < 0) {Serial.println(number); }
   return number;
}
}
void setup() {
  
  StandDrive.setMaxSpeed(5000); // <------------------------------------- та скорость до которой дв можно разогнать 9Ограничение сверху) 
  //StandDrive.setAcceleration(10.0);
  StandDrive.setSpeed(Sp);

  Serial.begin(9600);
  customKeypad.setHoldTime(1000); 
  customKeypad.setDebounceTime(250); //время на устранения дребезка контактов
}

// SOLUTION WITHOUT THREADS:
void loop(){
   
   //StandDrive.runSpeed(); // классическое вращение двигателя
   
   char key = customKeypad.getKey();

   do
      {
        StandDrive.runSpeed();
      }while (driver_flag == true );

   
   //buttoncheck(customKeypad, key);
 
   //Serial.print(key);
   

     // ----------------------------------Режим A:
     if (key == 65)
     {
      Serial.println("A button is pressed");
      Serial.println(permission);
      do {
      char key2 = customKeypad.getKey();
      V = buttoncheck(customKeypad, key2);
      } while (permission == false);
      permission = false; 
      Serial.println("finish");
      Serial.println(V);
      Serial.println(permission);
      
      delay (100);    
     }

     // -------------------------------------Режим B:
     if (key == 66)
     {
      Serial.println("B button is pressed");
      if (V == 0){ V = 150; }

      Serial.println (V);
      Sp = round(V * 20.1861); 
      Serial.println (Sp);
      StandDrive.setSpeed(Sp);
      
      do
      {
        StandDrive.runSpeed();
        Serial.println(customKeypad.getKeys());
      }while (customKeypad.getKeys() == false); 
      driver_flag = true;
      
     }

     // Режим C:
     if (key == 67)
     {
      Serial.println("С button is pressed");
      StandDrive.stop();
      driver_flag = false;
     }


     // Режим D:
     if (key == 68)
     {
      Serial.println("D button is pressed");
      if (V == 0){ V = 150; }
     // else { V = number;}
      
      V = V * (-1);
      Serial.println (V);
      Sp = round(V * 20.1861);
      Serial.println (Sp);
      StandDrive.setSpeed(Sp);
      
      do
      {
        StandDrive.runSpeed();
      }while (customKeypad.getKeys() == false ); //(key == ' ');
      driver_flag = true;
     }
}

// Если не получится то решать задачу через многопоточность
// Многопоточность:
/*
#include <Thread.h>  // подключение библиотеки ArduinoThread
const int soundPin = 3;  // переменная с номером пина пьезоэлемента
const int ledPin = 13;  // переменная с номером пина светодиода

Thread ledThread = Thread(); // создаём поток управления светодиодом
Thread soundThread = Thread(); // создаём поток управления сиреной

void setup() {
    pinMode(soundPin, OUTPUT); // объявляем пин 3 как выход.
    pinMode(ledPin, OUTPUT);   // объявляем пин 13 как выход.

    ledThread.onRun(ledBlink);  // назначаем потоку задачу
    ledThread.setInterval(1000); // задаём интервал срабатывания, мсек
    
    soundThread.onRun(sound);     // назначаем потоку задачу
    soundThread.setInterval(20); // задаём интервал срабатывания, мсек
}

void loop() {
    // Проверим, пришло ли время переключиться светодиоду:
    if (ledThread.shouldRun())
        ledThread.run(); // запускаем поток
    
    // Проверим, пришло ли время сменить тональность сирены:
    if (soundThread.shouldRun())
        soundThread.run(); // запускаем поток
}

// Поток светодиода:
void ledBlink() { 
    static bool ledStatus = false;    // состояние светодиода Вкл/Выкл
    ledStatus = !ledStatus;           // инвертируем состояние
    digitalWrite(ledPin, ledStatus);  // включаем/выключаем светодиод
}

 // Поток сирены:
void sound() { 
    static int ton = 100;  // тональность звука, Гц
    tone(soundPin, ton);  // включаем сирену на "ton" Гц
    if (ton <= 500) {  // до частоты 500 Гц 
        ton += 100;  // увеличиваем тональность сирены
    }
    else {  // по достижении 500 Гц
        ton = 100;  // сбрасываем тональность до 100 Гц
    }
}
*/
