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

// 49 = 1
// 50 = 2
// 51 = 3
// 52 = 4
// 53 = 5
// 54 = 6
// 55 = 7
// 56 = 8
// 57 = 9

*/

#include <Keypad.h>
#include <AccelStepper.h>
#include <Thread.h>

// Блок переменных по управлению двигателем:
AccelStepper StandDrive(1, 3, 2); // (number of motor, pin 3 = step, pin 2 = direction)
float V; // Скорость 150 по умолчанию (мм/с)
float Sp = round(V * 20.1861); 

// Подпрограмма считывания сигналов с клавиш:
const byte ROWS = 4; //число строк клавиатуры
const byte COLS = 4; //число столбцов клавиатуры
char hexaKeys[ROWS][COLS] = {
{'1','2','3','A'}, // здесь мы располагаем названия наших клавиш, как на клавиатуре,для удобства пользования
{'4','5','6','B'},
{'7','8','9','C'},
{'*','0','#','D'}
};

byte rowPins[ROWS] = {4, 5, 6, 7}; // выводы управления строками
byte colPins[COLS] = {8, 9, 10, 11}; // выводы управления столбцами
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// pin 12  -  резервный 

void buttoncheck(char key2){
  switch (key2) {
     case 65: //49
             Serial.println("A button is pressed");
             Serial.println (V);
             Serial.println(key2); // Передаем название нажатой клавиши в сериал порт
             delay (500);

             
     break;

     case 66: 
             Serial.println("B button is pressed");
             Serial.println (V);
             Serial.println(key2); // Передаем название нажатой клавиши в сериал порт
             delay (500);
             //StandDrive.run();
             
     break;

     case 67: 
             Serial.println("C button is pressed");
             Serial.println (V);
             Serial.println(key2); // Передаем название нажатой клавиши в сериал порт
             delay (500);
             
     break;

     case 68: 
             Serial.println("D button is pressed");
             Serial.println (V);
             Serial.println(key2); // Передаем название нажатой клавиши в сериал порт
             delay (500);
             //StandDrive.stop();
             
     break;    
     }
  
  }

void setup() {
  
  StandDrive.setMaxSpeed(2300);
  //StandDrive.setAcceleration(10.0);
  StandDrive.setSpeed(Sp);

  Serial.begin(9600);
}

// SOLUTION WITHOUT THREADS:
void loop(){
     //StandDrive.runSpeed(); // классическое вращение двигателя
     
     char key = customKeypad.getKey();
     
     // Режим A:
     if (key == 65)
     {
      Serial.println("A button is pressed");
      Serial.println (V);
      Serial.println(key); // Передаем название нажатой клавиши в сериал порт
      
      char key = customKeypad.getKey();
      delay (500);

      Serial.println('Key is: ', key);
      V = int(key);
      Serial.println ('Velocity is: ', V);
      
      // TO DO:
      // think how to read numbers into V
      /*
      while (key != 65){
        char key = customKeypad.getKey();
        if (key == 49){ V = key; Serial.println ('Velocity is: ', V);  }
      
      
        
        }

      */


      
    



      delay (500);
      while (key == '0')
      {
        StandDrive.runSpeed();
      }
      
     }

     // Режим B:
     if (key == 66)
     {
      Serial.println("B button is pressed");
      Serial.println (V);
      Serial.println('Key is: ',int(key)); // Передаем название нажатой клавиши в сериал порт
      

      if (V == 0){
        V = 150;
        }
        
      delay (500);
      while (key == '0')
      {
        StandDrive.runSpeed();
      }
      
     }

     // Режим C:
     if (key == 67)
     {
      Serial.println("С button is pressed");
      Serial.println (V);
      Serial.println(key); // Передаем название нажатой клавиши в сериал порт
      
      delay (500);
      while (key == ' ')
      {
        StandDrive.stop();
      }
      
     }


     // Режим D:
     if (key == 68)
     {
      Serial.println("D button is pressed");
      
      Serial.println(key); // Передаем название нажатой клавиши в сериал порт
      V = V * (-1);

      Serial.println (V);
      delay (500);
      while (key == '0')
      {
        StandDrive.runSpeed();
      }
      
     }

     

     
}

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
