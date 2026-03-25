#include <iostream>
#include <conio.h>
#include <string>
#include <cstdlib>
#include <vector>

#include "classes.h"

using namespace std;

wstring inputStationName() {
	wstring stationName;

	wcout << L"Введите название своей будущей станции (max: 16): ";

	getline(wcin, stationName);

	wcout << "\n";

	if (stationName.length() > 16) stationName = stationName.substr(0, 16);

	return stationName;
}

wstring getRandomName(wstring type) {
	wstring names[25] = {
		L"Sonya",
		L"Tonya",
		L"Vanya",
		L"Vitya",
		L"Sanya",
		L" Master",
		L" Centner",
		L" Tualet",
		L" Don",
		L" Barebuh",
		L"A-",
		L"B-",
		L"C-",
		L"D-",
		L"E-",
		L"Alpha",
		L"Beta",
		L"Delta",
		L"Sin",
		L"Cos",
		L"Mopty",
		L"Soply",
		L"Voply",
		L"Joply",
		L"Popki"
	};
	if (type == L"KEEPER") {
		int indexKeeper1 = rand() % 5;
		int indexKeeper2 = rand() % 5 + 5;
		wstring KeeperName = names[indexKeeper1] + names[indexKeeper2];
		return KeeperName;
	}
	else if (type == L"INTEGRATOR") {
		int indexIntegrator1 = rand() % 5 + 10;
		int indexIntegrator2 = rand() % 5 + 15;
		wstring IntegratorName = names[indexIntegrator1] + names[indexIntegrator2];
		return IntegratorName;
	}
	if (type == L"OPTY") {
		wstring opty = L"Opty-";
		int indexOpty1 = rand() % 5 + 20;
		wstring OptyName = opty + names[indexOpty1];
		return OptyName;
	}
}

wstring getSpace(int max, int min) {
	wstring result = L"";
	wstring one = L" ";
	while (min < max) {
		result += one;
		min++;
	}
	return result;
}

int getChoice() {
	int choice = _getch();

	while ((choice < '0') || (choice > '7')) {
		wcout << L"\nНеизвестная команда! Попробуйте ещё раз...\n";
		wcout << L"Введите команду > ";
		choice = _getch();
	}

	return choice;
}

int randomHundred() {
	int chance = rand() % 100 + 1;
	return chance;
}

int sostSpace(int input) {
	if (input == 100) return 3;
	else if ((input < 100) && (input >= 10)) return 2;
	else return 1;
}

void printRobots(Station& station) {
	wcout << L"                      [НАСЕЛЕНИЕ (" << station.used() << L"/" << station.capacity() << L")]\n";
	wcout << L"ID | ТИП        | ИМЯ           | ШАССИ (HP) | ПО (SP) | СТАТУС\n";
	wcout << L"----------------------------------------------------------------\n";
	int t = 1;
	for (auto& r : station.getRobots()) {

		wstring typeRobot = r->getType();

		wstring proc;
		if (r->GetSost().first >= 50) proc = L"OK";
		else if ((r->GetSost().first >= 25) && (r->GetSost().first < 50)) proc = L"WARN";
		else if (r->GetSost().first < 25) proc = L"CRIT";

		wstring active;
		if (r->getActive() == 1) active = L"Активен";
		else active = L"Мёртв";

		wcout << L"#" << t << L" | " << typeRobot << getSpace(10, typeRobot.length()) << L" | " <<
			r->getName() << getSpace(13, r->getName().length()) << L" | [" << r->GetSost().first << L"] " <<
			getSpace(3, sostSpace(r->GetSost().first)) << proc << getSpace(4, proc.length()) << L" | [" <<
			r->GetSost().second << L"]  " << getSpace(3, sostSpace(r->GetSost().second)) << " | " << active << endl;
		t++;
	}
}

void printModuls(Station& station) {
	wcout << L"                          [МОДУЛИ]\n";
	int t = 1;
	int used = station.used();
	for (auto& m : station.getModules()) {

		wstring typeModule = m->getType();

		wstring active;
		if (m->getActive() == 1) active = L"АКТИВЕН";
		else active = L"ОТКЛЮЧЁН";

		int energy = m->GetEnergyOutput() - m->GetEnergyInput();
		int data = m->GetDataOutput() - m->GetDataInput();

		wcout << L"[" << t << L"] " << typeModule << getSpace(15, typeModule.length()) << L" (Lvl " << m->getLevel() <<
			L")   : ";
		if (typeModule == L"Жилой отсек") {
			wcout << L"Мест ";
			if (used <= 8) {
				wcout << used;
			}
			else {
				wcout << L"8";
				used -= 8;
			}
			wcout << L"/8" << endl;
		}
		else wcout << active << L" (" << energy << L"E, " << data << L"B)" << endl;
		t++;
	}
}

void printMoney(Station& station) {
	wcout << L"                          [РЕСУРСЫ]\n";
	wcout << L"Энергия: " << station.getTotalEnergy() << endl;
	wcout << L"Биты: " << station.getTotalData() << endl;
	wcout << L"Шанс сигнала: " << (int)station.CalculateSignalChance() << L"% (Мощность: " << station.getTotalPower() <<
		L" | Помехи : " << station.getRobots().size() * 10 << L")\n";
}

void printStation(Station& station, wstring nameStation) {
	int day = station.getDay();
	wcout << L"          ┌──────────────────────────────────────────┐\n";
	wcout << L"          |  СТАНЦИЯ: \"" << nameStation << L"\"" << getSpace(16, nameStation.length()) << L"\ | ДЕНЬ: " << day;
	if (day < 10) wcout << L"  ";
	else if (day < 100) wcout << L" ";
	else if (day < 1000) wcout << L"";
	wcout << L" |" << endl;
	wcout << L"          └──────────────────────────────────────────┘\n";
}

void getChoiceModuls(Station& station) {
	wcout << L"Введите номер модуля: ";
	int choice;
	wcin >> choice;

	while ((choice < 1) || (choice > station.getModules().size())) {
		wcout << L"\nНеизвестный номер! Попробуйте ещё раз...\n";
		wcout << L"Введите номер модуля: ";
		choice = _getch();
	}

	if (station.getModules()[choice - 1]->getType() == L"Жилой отсек") {
		wcout << L"\nЖилой отсек он на то и жилой, бро...\n";
		return;
	}

	wcout << station.getModules()[choice - 1]->getType() << L" был " << station.getModules()[choice - 1]->tumb() << endl;
}

void utilsAddRobot(Station& station) {
	wcout << L"                      [СОЗДАНИЕ РОБОТОВ]\n";
	wcout << L"1. KEEPER\n";
	wcout << L"2. INTEGRATOR\n";
	wcout << L"3. OPTY\n";
	wcout << L"Выберете желаемый тип робота: ";

	int choice;

	wcin >> choice;

	wcout << L"\n";

	while ((choice < 1) || (choice > 3)) {
		wcout << L"Ошибка! Попробуйте ещё раз: \n";
		wcout << L"1. KEEPER\n";
		wcout << L"2. INTEGRATOR\n";
		wcout << L"3. OPTY\n";
		wcout << L"Выберете желаемый тип робота: ";

		wcin >> choice;

		wcout << L"\n";
	}

	switch (choice) {
	case 1:
		if (!station.AddRobot(L"KEEPER")) {
			wcout << L"Ошибка! Кажется, у вас недостаточно средств...\n";
		}
		else wcout << L"Новый робот успешно создан!\n";
		break;
	case 2:
		if (!station.AddRobot(L"INTEGRATOR")) {
			wcout << L"Ошибка! Кажется, у вас недостаточно средств...\n";
		}
		else wcout << L"Новый робот успешно создан!\n";
		break;
	case 3:
		if (!station.AddRobot(L"OPTY")) {
			wcout << L"Ошибка! Кажется, у вас недостаточно средств...\n";
		}
		else wcout << L"Новый робот успешно создан!\n";
		break;
	}
}

void getChoiceBuildModuls(Station& station) {
	wcout << L"                      [СОЗДАНИЕ МОДУЛЕЙ]\n";
	wcout << L"1. Архив\n";
	wcout << L"2. Командный отсек\n";
	wcout << L"3. Жилой отсек\n";
	wcout << L"4. Генераторная\n";
	wcout << L"Выберете желаемый модуль: ";

	int choice;

	wcin >> choice;

	wcout << L"\n";

	while ((choice < 1) || (choice > 4)) {
		wcout << L"Ошибка! Попробуйте ещё раз: \n";
		wcout << L"                      [СОЗДАНИЕ МОДУЛЕЙ]\n";
		wcout << L"1. Архив\n";
		wcout << L"2. Командный отсек\n";
		wcout << L"3. Жилой отсек\n";
		wcout << L"4. Генераторная\n";
		wcout << L"Выберете желаемый модуль: ";

		wcin >> choice;

		wcout << L"\n";
	}
	switch (choice) {
	case 1:
		if (!station.AddModule(L"Архив")) {
			wcout << L"Ошибка! Кажется, у вас недостаточно средств...\n";
		}
		else wcout << L"Архив успешно создан!\n";
		break;
	case 2:
		if (!station.AddModule(L"Командный отсек")) {
			wcout << L"Ошибка! Кажется, у вас недостаточно средств...\n";
		}
		else wcout << L"Командный отсек успешно создан!\n";
		break;
	case 3:
		if (!station.AddModule(L"Жилой отсек")) {
			wcout << L"Ошибка! Кажется, у вас недостаточно средств...\n";
		}
		else wcout << L"Жилой отсек успешно создан!\n";
		break;
	case 4:
		if (!station.AddModule(L"Генераторная")) {
			wcout << L"Ошибка! Кажется, у вас недостаточно средств...\n";
		}
		else wcout << L"Генераторная успешно создана!\n";
		break;
	}
}

void getChoiceUpgrateModuls(Station& station) {

}