#include <iostream>
#include <string>
#include <conio.h>
#include <utility>
#include <cstdlib>

#include "utils.h"
#include "classes.h"

using namespace std;

int Game() {
	int stop = 0;
	int choice;

	wstring nameStation = inputStationName();

	Station station;
	station.Init();

	while (!stop) {
		if (!station.ProcessDay()) {
			stop = 1;
			continue;
		}
		system("cls");
		printStation(station, nameStation);
		printMoney(station);
		printModuls(station);
		printRobots(station);
		station.PrintEvents();

		int stopMenu = 0;
		while (!stopMenu) {
			wcout << L"   ┌────────────────────МЕНЮ УПРАВЛЕНИЯ─────────────────────┐\n";
			wcout << L"   | 1. Следующий день (Списать ресурсы)                    |\n";
			wcout << L"   | 2. Создать нового робота (Цена: 150 энергии 200 битов) |\n";
			wcout << L"   | 3. Управление модулями (Вкл/Выкл)                      |\n";
			wcout << L"   | 4. Построить модуль                                    |\n";
			wcout << L"   | 5. Улучшить модуль                                     |\n";
			wcout << L"   | 6. Список всех роботов                                 |\n";
			wcout << L"   | 7. Список всех модулей                                 |\n";
			wcout << L"   | 0. Выход                                               |\n";
			wcout << L"   └────────────────────────────────────────────────────────┘\n";

			wcout << L"Введите команду > ";
			choice = getChoice();
			wcout << L"\n";

			switch (choice) {
			case '1':
				stopMenu = 1;
				break;
			case '2':
				system("cls");
				printStation(station, nameStation);
				printMoney(station);
				printRobots(station);
				utilsAddRobot(station);
				break;
			case '3':
				system("cls");
				printStation(station, nameStation);
				printMoney(station);
				printModuls(station);
				getChoiceModuls(station);
				break;
			case '4':
				system("cls");
				printStation(station, nameStation);
				printMoney(station);
				printModuls(station);
				getChoiceBuildModuls(station);
				break;
			case '5':
				system("cls");
				printStation(station, nameStation);
				printMoney(station);
				getChoiceUpgrateModuls(station);
				break;
			case '6':
				system("cls");
				printStation(station, nameStation);
				printMoney(station);
				printRobots(station);
				break;
			case '7':
				system("cls");
				printStation(station, nameStation);
				printMoney(station);
				printModuls(station);
				break;
			case '0':
				system("cls");
				wcout << L"Выполняется выход...\n\n";
				stop = 1;
				stopMenu = 1;
				break;
			default:
				break;
			}
			if (stopMenu == 1) continue;
		}

		if (stop == 1) continue;
	}

	return 1;
}