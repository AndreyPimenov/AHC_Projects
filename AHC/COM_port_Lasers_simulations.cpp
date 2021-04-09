// программа нажатия клавиш
/*
http://www.realcoding.net/article/view/1857
key  | int | Hex  | symbolic name
_________________________________
F1   | 112 | 0x70 | VK_F1
F4   | 115 | 0x73 | VK_F4
Ctrl |  17 | 0x11 | VK_CONTROL
Alt  |  18 | 0x12 | VK_MENU
Tab  |   9 | 0x9  | VK_TAB

Win(Л)	91	0x5B	VK_LWIN
1	    97	0x61	VK_NUMPAD1
2	    98	0x62	VK_NUMPAD2



Alt + Tab - смена окон
Ctrl + F4 - стоп
Ctrl + F1 - старт

Инициализация: Alt + Tab + Tab переведет с основного цикла программы на окно лазера, между оканами лазеров достаточно переключать Alt+Tab 
*/

// Рекомендация по запуску:
/*
Для корректной работы программы:
1. Проверить надежность всех физических подключений
2. Проверить настройки COM порта: WIN -> Settings -> Device Manager -> СOM 1 (на двух компьютерах скорость передачи должна совпадать, как междук собой так и в программах)
3. На момент запуска оставить три активныъх окна, открытых в хронологическом порядке: окно лазера №2, окно лазера №1, Microsoft Visual Studio 2017 (из которого программу нужно запустить).
*/

#include <Windows.h>
#include <iostream>

//#include "SerialGate.h"
#include <conio.h>
#include <Serial.h>

using namespace std;

int k = 0;
void Lasers ();

/*
void Lasers() //Функции стоп/старта лазеров
{
	printf("starting perforation . . . "); // нужно для того, чтобы уйти в консоль


    //1. Перейти из окна MSV в окно 1-го 
	keybd_event(VK_MENU, 0, 0, 0); Sleep(150); //имитация НАЖАТИЯ клавиши ALT и задержка на срабатывание
	keybd_event(VK_TAB, 0, 0, 0); Sleep(150);
	keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0); Sleep(150); //один раз Tab: MVS
	keybd_event(VK_TAB, 0, 0, 0); keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0); //второй раз Tab: L1
	Sleep(150);
	keybd_event(VK_MENU, 0, KEYEVENTF_KEYUP, 0); //имитация ОТПУСКАНИЯ клавиши Alt - запуск выбранного окна 
	Sleep(150);

	//2. первого лазера
	//СТОП: Ctrl + F4 
	keybd_event(VK_CONTROL, 0, 0, 0); //имитация НАЖАТИЯ клавиши Ctrl
	Sleep(150);
	keybd_event(VK_F4, 0, 0, 0);	  //имитация НАЖАТИЯ клавиши F4
	Sleep(150);
	keybd_event(VK_F4, 0, KEYEVENTF_KEYUP, 0);
	Sleep(150);
	keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0); //имитация ОТПУСКАНИЯ клавиши
	Sleep(150);

	//СТАРТ: Ctrl + F1
	keybd_event(VK_CONTROL, 0, 0, 0); //имитация НАЖАТИЯ клавиши Ctrl
	Sleep(150);
	keybd_event(VK_F1, 0, 0, 0);      //имитация НАЖАТИЯ клавиши F1
	Sleep(150);
	keybd_event(VK_F1, 0, KEYEVENTF_KEYUP, 0);
	Sleep(150);
	keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0); //имитация ОТПУСКАНИЯ клавиши
	Sleep(300);

	//ПЕРЕКЛЮЧЕНИЕ: Alt+Tab (окно 2-го будет через три окна от 1-го
	keybd_event(VK_MENU, 0, 0, 0); //имитация НАЖАТИЯ клавиши ALT
	Sleep(150);
	keybd_event(VK_TAB, 0, 0, 0); Sleep(150); keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);
	Sleep(150);
	keybd_event(VK_TAB, 0, 0, 0); Sleep(150); keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);
	Sleep(150);
	keybd_event(VK_TAB, 0, 0, 0); Sleep(150); keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);
	Sleep(150);
	keybd_event(VK_MENU, 0, KEYEVENTF_KEYUP, 0); //имитация ОТПУСКАНИЯ клавиши Alt - запуск выбранного окна 
	Sleep(500);

	// второго лазера
	//СТОП: Ctrl + F4 
	keybd_event(VK_CONTROL, 0, 0, 0); //имитация НАЖАТИЯ клавиши Ctrl
	Sleep(150);
	keybd_event(VK_F4, 0, 0, 0);	  //имитация НАЖАТИЯ клавиши F4
	Sleep(150);
	keybd_event(VK_F4, 0, KEYEVENTF_KEYUP, 0);
	Sleep(150);
	keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0); //имитация ОТПУСКАНИЯ клавиши
	Sleep(150);

	//СТАРТ: Ctrl + F1
	keybd_event(VK_CONTROL, 0, 0, 0); //имитация НАЖАТИЯ клавиши Ctrl
	Sleep(150);
	keybd_event(VK_F1, 0, 0, 0);      //имитация НАЖАТИЯ клавиши F1
	Sleep(150);
	keybd_event(VK_F1, 0, KEYEVENTF_KEYUP, 0);
	Sleep(150);
	keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0); //имитация ОТПУСКАНИЯ клавиши
	Sleep(300);


	// возвращение в Microsoft Visual Studio
	keybd_event(VK_MENU, 0, 0, 0); //имитация НАЖАТИЯ клавиши ALT и задержка на срабатывание
	Sleep(150);
	keybd_event(VK_TAB, 0, 0, 0); Sleep(150); keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0); //один раз Tab: MVS
	Sleep(150);
	keybd_event(VK_MENU, 0, KEYEVENTF_KEYUP, 0); //имитация ОТПУСКАНИЯ клавиши Alt - запуск выбранного окна 
	Sleep(150);

	keybd_event(VK_MENU, 0, 0, 0); //имитация НАЖАТИЯ клавиши ALT и задержка на срабатывание
	Sleep(150);
	keybd_event(VK_TAB, 0, 0, 0); Sleep(150); keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0); //один раз Tab: MVS
	Sleep(150);
	keybd_event(VK_TAB, 0, 0, 0); Sleep(150);  keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0); //второй раз Tab: L1
	Sleep(150);
	keybd_event(VK_MENU, 0, KEYEVENTF_KEYUP, 0); //имитация ОТПУСКАНИЯ клавиши Alt - запуск выбранного окна 
	Sleep(150);


	printf("perforation is finished \n", k); //для выхода в консоль


}
*/


 void Lasers() //Функции стоп/старта лазеров
{
	printf("starting perforation . . . "); // нужно для того, чтобы уйти в консоль

	 //1. Перейти в окно первого лазера
	 keybd_event(VK_LWIN, 0, 0, 0); 
	 keybd_event(0x31, 0, 0, 0);
	 keybd_event(0x31, 0, KEYEVENTF_KEYUP, 0);
	 keybd_event(VK_LWIN, 0, KEYEVENTF_KEYUP, 0); 
	 Sleep(25);  
	//СТОП: Ctrl + F4 
	keybd_event(VK_CONTROL, 0, 0, 0); //имитация НАЖАТИЯ клавиши Ctrl
	keybd_event(VK_F4, 0, 0, 0);	  //имитация НАЖАТИЯ клавиши F4
	keybd_event(VK_F4, 0, KEYEVENTF_KEYUP, 0);
	Sleep(25);  
	//СТАРТ: Ctrl + F1
	keybd_event(VK_F1, 0, 0, 0);      //имитация НАЖАТИЯ клавиши F1
	keybd_event(VK_F1, 0, KEYEVENTF_KEYUP, 0);
	keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0); //имитация ОТПУСКАНИЯ клавиши
	Sleep(25); 
	

	//2. Перейти в окно второго лазера
	keybd_event(VK_LWIN, 0, 0, 0);
	keybd_event(0x32, 0, 0, 0); 
	keybd_event(0x32, 0, KEYEVENTF_KEYUP, 0);
	keybd_event(VK_LWIN, 0, KEYEVENTF_KEYUP, 0);
	Sleep(25); 
	//СТОП: Ctrl + F4 
	keybd_event(VK_CONTROL, 0, 0, 0); //имитация НАЖАТИЯ клавиши Ctrl
	keybd_event(VK_F4, 0, 0, 0);	  //имитация НАЖАТИЯ клавиши F4
	keybd_event(VK_F4, 0, KEYEVENTF_KEYUP, 0);
	Sleep(25);  
	//СТАРТ: Ctrl + F1
	keybd_event(VK_F1, 0, 0, 0);      //имитация НАЖАТИЯ клавиши F1
	keybd_event(VK_F1, 0, KEYEVENTF_KEYUP, 0);
	keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0); //имитация ОТПУСКАНИЯ клавиши
	Sleep(25); 

	printf("perforation is finished \n", k); //для выхода в консоль

}

 void ChinaLasers() {
	 printf("ChinaLasers begin reloading ... ");

	 // Отключение: ALT + F4 (small delay)   ---> ALT+F4 (small delay)
	 // WIN + 3:
	 keybd_event(VK_LWIN, 0, 0, 0);
	 keybd_event(0x31, 0, 0, 0); 
	 keybd_event(0x31, 0, KEYEVENTF_KEYUP, 0);
	 keybd_event(VK_LWIN, 0, KEYEVENTF_KEYUP, 0);
	 Sleep(125);

	 // Enter (small delay):

	 keybd_event(VK_RETURN, MapVirtualKeyA(VK_RETURN, 0), 0, 0);
	 keybd_event(VK_RETURN, MapVirtualKeyA(VK_RETURN, 0), KEYEVENTF_KEYUP, 0);
	 Sleep(125);

	 // ALT + F4 (small delay): 
	 keybd_event(VK_MENU, 0, 0, 0); 
	 keybd_event(VK_F4, 0, 0, 0);	  
	 keybd_event(VK_F4, 0, KEYEVENTF_KEYUP, 0);
	 Sleep(125);

	 // ALT + F4 (small delay):
	 keybd_event(VK_MENU, 0, 0, 0); 
	 keybd_event(VK_F4, 0, 0, 0);	  
	 keybd_event(VK_F4, 0, KEYEVENTF_KEYUP, 0);
	 Sleep(225);

	 // Подключение: WIN+3 ---> Enter (delay) ---> ALT+L (small delay) +Enter

	 // WIN + 3:
	 keybd_event(VK_LWIN, 0, 0, 0);
	 keybd_event(0x31, 0, 0, 0); 
	 keybd_event(0x31, 0, KEYEVENTF_KEYUP, 0);
	 keybd_event(VK_LWIN, 0, KEYEVENTF_KEYUP, 0);
	 Sleep(10000);

	 // Enter (small delay):
	 keybd_event(VK_RETURN, 0, 0, 0);
	 keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
	 Sleep(125);

	 // Enter (small delay):
	 /*
	 keybd_event(VK_RETURN, MapVirtualKeyA(VK_RETURN, 0), 0, 0);
	 keybd_event(VK_RETURN, MapVirtualKeyA(VK_RETURN, 0), KEYEVENTF_KEYUP, 0);
	 Sleep(125);
	 */

	 // ALT+L (small delay)
	 keybd_event(VK_MENU, 0, 0, 0);
	 keybd_event(0x4C, 0, 0, 0);
	 keybd_event(0x4C, 0, KEYEVENTF_KEYUP, 0);
	 keybd_event(VK_MENU, 0, KEYEVENTF_KEYUP, 0);
	 Sleep(525);

	 // Enter (small delay):
	 keybd_event(VK_RETURN, 0, 0, 0);
	 keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
	 Sleep(125);
	 /*
	 keybd_event(VK_RETURN, MapVirtualKeyA(VK_RETURN, 0), 0, 0);
	 keybd_event(VK_RETURN, MapVirtualKeyA(VK_RETURN, 0), KEYEVENTF_KEYUP, 0);
	 Sleep(125);
	 */

	 printf("ChinaLasers reloading is finished ");
 }
// Устаревшая библиотека: 
 /*
 Источник: http://www.kernelchip.ru/pcports/PS057.php
 SerialGate.dll - динамическая библиотека для работы с COM портами на C++
 1) использует классы: SerialGate
 2) определяет все установленные в системе COM порты
 3) корректо работает как с виртуальными, так и с реальными портами
 
 Описание функций класса Serial Gate:

 
 bool Open(int port, int baud);
 Открывает доступ к COM порту с номером port на скорости baud байт/с.
 true - порт существует и не занят другим приложением 
 false - иначе

 Пример исползования:
 Serial sg;

 bool b = sg.Open(1, 19200);
 if (b == true)
 {
    // port is open sucsesfully
 }
 else
 {
    // port open error
 }
 -----------------------------


 int Send(char* buff, int szBuff);
 Функция записывает в ранее открытый порт szBuff байт данных из буфера buff.
 Возврашщает число успешно записанных байт данных в порт.

 Пример использования:

 char buff[256];
 for (int i=0; i < sizeoff(buff); i++)
 {
 buff[i] = i;
 }

 int SendCounter = sg.Send(buff, sizeoff(buff));

 if (SendCounter != sizeoff(buff))
 {
 // не все данные были записаны в порт
 }
 -----------------------------


 int Recv(char* buff, int szBuff);
 Читаем из ранее открытого порта szBuff байт даннызх и помещает их в буфер bufer
 Возвращает число реально прочитанных байт данных

 Пример использования:

 char buff[256];
 int RcvCounter = sg.Recv(buff, sizeof(buff));

 if (RcvCounter != sizeof(buff))
 {
   // прочли меньше чем заказывали
 }
 -----------------------------
 

 void SetLine(OUT_LINES_NAME In, bool state);
 Функция устанавливает в логическую 0 или логическую 1 одну из выходных
 сигнальных линий, а именно DTR или RTS.
 Имя линии задается через переключатель OUT_LINES_NAME.
 Вторым параметром задается состояние (true - 1, false - 0) в которое необходимо перевести линию

 Пример использования: 
 sg.SetLine(sg.RTS, true); //установит на линии RTS лог.1
 -----------------------------

 bool GetLine(IN_LINES_NAME ln);
 Функция возвращает состояние одной из входных сигнальных линий 
 (CTS, DSR, RING или RLSD).
 Имя линии задается через перечислитель IN_LINES_NAME.

 Пример использованиея:
 boll b = sg.GetLine(sg.DSR); // читаем состояние линии DSR
 -----------------------------

 void GetPortsInfo(PortInfo* pi);	
Функция заполняет переданную ей структуру PortInfo
информацией о установленных в системе COM портах.
 -----------------------------

 void Clean();
 Ощищает входной и выходной буфер данных COM порта.
 Пример использованиея:
 sg.Clean();
 -----------------------------

 void Close();
 Закрывает ранее установленное соединение с COM портом.
 Пример использованиея:
 sg.Close();

 */
 
/*
 //Севрерная программа для работы с COM-портом
 int main()
 {
 int port = 1;
 int rate = 9600;

 SerialGate sg;

 bool res = sg.Open(port, rate);
 if (res == false)
 {
 printf("Open Error.. \n");
 _getch();
 return 0;
 }
 else
 {
 printf("Open OK!..\n");
 }

 printf("Press key to send it to COM port.\n");
 printf("Press '#' to exit.\n\n");

 while (true)
 {
 char c = _getch();
 printf("%c", c);

 sg.Send(&c, sizeof(c));

 if (c == '#')
 break;
 }

 sg.Close();

 _getch();
 return 0;
 }
 */

/*
Памятка подключения библиотек:
0. Properties -> All Configuration
1. C/C++ -> General -> Additional Include Directories -> (path Iclude" folder in OpenCV) -> Apply
2. Linker -> Additionla Library Direction -> (example Opencv/build/x64/lib)
3. Input -> Additional Dependencies (we need to add the names of OpenCV libs): One lib to Debug and One lib to Release
4. Check compilation
5. link the ddl files:
5a Copy & Past dll files to project folder
5b Copy and Past the Path of dll files on Windows System variables: Advanced System Settings -> Environmental Variables -> Path -> Edit -> Add the Path of dll files
*/


int main()
{
	
	Serial com("\\\\.\\COM1", 57600, 8, 'N', 1);
	printf("Opening port %s.\n", com.GetPort().c_str());
	if (com.Open() == 0) { printf("OK.\n");}
	else { printf("Eror.\n"); return 1;}
	
	bool successFlag;
	/*
	printf("Writing.\n");
	char s[] = "Hello";
	successFlag = com.Write(s); //write string
	successFlag = com.WriteChar('!'); //write a character
	//printf("Waiting 3 seconds.\n");
	//delay(5000); //delay 5 sec to wait for a character
	*/

	/*
	while (true) {
		Lasers();
		delay(1000);
	}
	*/
	
	
	while (true) {
		printf("Port Reading...\n");
		char c = com.ReadChar(successFlag); //считать символ
		if (successFlag) //printf("Rx: %c\n", c);
		{
			if (c == 'a')  Lasers(); //вызов функции стоп/старта лазеров для обнуления ошибки рассогласования
			if (c == 'b') ChinaLasers(); // Перезапуск КЛГ

		}
		
	}
	
	printf("Closing port %s.\n", com.GetPort().c_str());
	com.Close();
	

	
	
	system("pause"); // для отладки

	return 0; // для работы 
}

 
