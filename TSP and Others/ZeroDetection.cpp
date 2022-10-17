// Программа для анализа полученные от схемы даннных:
// file_read.cpp: определяет точку входа для консольного приложения.
// #include "stdafx.h"

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

float str_to_float (string line) {
	/*
		Данная функция преобразует строку из отчета в число 
	*/
	float result = (   (int(line[3]) - 48) * 10 + (int(line[4]) - 48) + (int(line[6]) - 48) *.1 + (int(line[7]) - 48) *.01 + (int(line[8]) - 48) * .001);
	return result;
}

// формирование динамического массива, для всех измерений

// поиск уникальных их должно быть 50 на 1 секунду



int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "rus"); 
	char buff[50]; // буфер промежуточного хранения считываемого из файла текста
	ifstream file_txt; 
	string file_line;
	file_txt.open("C:/Users/andrey.pimenov/Desktop/TSP_Detecting_Zero/delay-10.txt");

	if (!file_txt.is_open()) // если файл не открыт
		cout << "Файл не может быть открыт!\n"; // сообщить об этом
	else
	{
		
		/*
		file_txt >> buff; // считали первое слово из файла
		cout << buff << endl; 

		file_txt.getline(buff, 50); // считали строку из файла
		file_txt.close(); // закрываем файл
		cout << buff << endl; // напечатали эту строку
		*/

		while (getline(file_txt, file_line))
		{
			string line = file_line.substr(3, 10);
	
			// cout << "initial" << line << '\n';
			cout << str_to_float(line) << endl;


		}
		file_txt.close();

	}

	system("pause");
	return 0;
}
