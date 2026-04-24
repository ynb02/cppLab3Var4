#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <string>
#include <cstdlib>
#include <vector>
#include <cmath>

#include "robot.h"
#include "module.h"
#include "station.h"

using namespace std;

void clearScreen() {
    cout << "\033[2J\033[H";
    cout.flush();
}

static int getch_unix() {
    struct termios oldt, newt;
    int ch;
    if (tcgetattr(STDIN_FILENO, &oldt) < 0) return -1;
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    newt.c_cc[VMIN] = 1;
    newt.c_cc[VTIME] = 0;
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &newt) < 0) return -1; // TCSAFLUSH вместо TCSANOW — сбрасывает буфер!
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

string inputStationName() {
    string stationName;

    cout << "Введите название своей будущей станции (max: 16): ";
    cout.flush();

    getline(cin, stationName);

    cout << "\n";

    if (stationName.length() > 16) stationName = stationName.substr(0, 16);

    return stationName;
}

string getRandomName(string type) {
    string names[25] = {
        "Sonya", "Tonya", "Vanya", "Vitya", "Sanya",
        " Master", " Centner", " Tualet", " Don", " Barebuh",
        "A-", "B-", "C-", "D-", "E-",
        "Alpha", "Beta", "Delta", "Sin", "Cos",
        "Mopty", "Soply", "Voply", "Joply", "Popki"
    };

    if (type == "KEEPER") {
        int i1 = rand() % 5;
        int i2 = rand() % 5 + 5;
        return names[i1] + names[i2];
    } else if (type == "INTEGRATOR") {
        int i1 = rand() % 5 + 10;
        int i2 = rand() % 5 + 15;
        return names[i1] + names[i2];
    } else if (type == "OPTY") {
        int i1 = rand() % 5 + 20;
        return "Opty-" + names[i1];
    }
    return "";
}

int utf8_length(const string& s) {
    int count = 0;
    for (size_t i = 0; i < s.size(); i++) {
        // считаем только начало символа (не continuation byte)
        if ((s[i] & 0b11000000) != 0b10000000) {
            count++;
        }
    }
    return count;
}

string getSpace(int max, int min) {
    string result = "";
    while (min < max) {
        result += " ";
        min++;
    }
    return result;
}

int getChoice(char num) {
    int choice = getch_unix();

    while ((choice < '0') || (choice > num)) {
        cout << "\nНеизвестная команда! Попробуйте ещё раз...\n";
        cout << "Введите команду > ";
        cout.flush();
        choice = getch_unix();
    }

    return choice;
}

int randomHundred() {
    return rand() % 100 + 1;
}

int sostSpace(int input) {
    if (input == 100) return 3;
    else if ((input < 100) && (input >= 10)) return 2;
    else return 1;
}

void printRobots(Station& station) {
    int t = 1;
    cout << "                      [НАСЕЛЕНИЕ (" << station.used() << "/" << station.capacity() << ")]\n";
    cout << "ID | ТИП        | ИМЯ           | ШАССИ (HP) | ПО (SP) | СТАТУС  | ВОЗРАСТ\n";
	cout << "----------------------------------------------------------------------------\n";
    for (auto& r : station.getRobots()) {
        cout << "#" << t << " | ";
        cout << r;
        t++;
    }
}

void printModuls(Station& station) {
    cout << "                          [МОДУЛИ]\n";
    int t = 1;
    int used = station.used();
    double count = (used > 0) ? ceil((double)station.capacity() / used) : 0;
    int numLive = 1;
    for (auto& m : station.getModules()) {
        cout << "[" << t << "] ";
        cout << m;
        if (m->getType() == "Жилой отсек") {
            int cap = 6 + 2 * m->getLevel();
            int occupants;
            if ((used > 8) && (numLive < count) && (numLive != 0)) {
                occupants = cap; numLive++; used -= cap;
            } else if (numLive == 0) {
                occupants = 0;
            } else {
                occupants = used; numLive = 0;
            }
            cout << " [" << occupants << "/" << cap << "]\n";
        }
        t++;
    }
}

void printModulsList(Station& station, bool status) {
    int t = 1;
    for (auto& m : station.getModules()) {
        string typeModule = m->getType();

        cout << t << ". " << typeModule << getSpace(16, typeModule.length()) << m->getLevel() << " LVL ";

        if (status == true) {
            cout << "(" << m->getUpgradePrice().first << " E, " << m->getUpgradePrice().second << " D)";
        }

        cout << "\n";
        t++;
    }
}

void printMoney(Station& station) {
    cout << "                          [РЕСУРСЫ]\n";
    cout << "Энергия: " << station.getTotalEnergy() << "\n";
    cout << "Биты: " << station.getTotalData() << "\n";
    cout << "Шанс сигнала: " << (int)station.CalculateSignalChance() << "% (Мощность: " << station.getTotalPower() <<
        "E | Помехи : " << station.getRobots().size() * 10 << ")\n";
}

void printStation(Station& station, string nameStation) {
    int day = station.getDay();
    cout << "          ┌──────────────────────────────────────────┐\n";
    cout << "          |  СТАНЦИЯ: \"" << nameStation << "\"" << getSpace(16, utf8_length(nameStation)) << " | ДЕНЬ: " << day;
    if (day < 10) cout << "  ";
    else if (day < 100) cout << " ";
    cout << " |\n";
    cout << "          └──────────────────────────────────────────┘\n";
}

void getChoiceModuls(Station& station) {
    cout << "Введите номер модуля: ";
    cout.flush();
    int choice;
    cin >> choice;

    while ((choice < 1) || (choice > (int)station.getModules().size())) {
        cout << "Неизвестный номер! Попробуйте ещё раз...\n";
        cout << "Введите номер модуля: ";
        cout.flush();
        cin >> choice;
    }

    if (station.getModules()[choice - 1]->getType() == "Жилой отсек") {
        cout << "Жилой отсек он на то и жилой, бро...\n";
        return;
    }

    cout << station.getModules()[choice - 1]->getType() << " был " << station.getModules()[choice - 1]->tumb() << "\n";
}

void utilsAddRobot(Station& station) {
    cout << "                      [СОЗДАНИЕ РОБОТОВ]\n";
    cout << "1. KEEPER\n";
    cout << "2. INTEGRATOR\n";
    cout << "3. OPTY\n";
    cout << "Выберете желаемый тип робота: ";
    cout.flush();

    int choice;
    cin >> choice;

    while ((choice < 1) || (choice > 3)) {
        cout << "Ошибка! Попробуйте ещё раз: \n";
        cout << "1. KEEPER\n2. INTEGRATOR\n3. OPTY\n";
        cout << "Выберете желаемый тип робота: ";
        cout.flush();
        cin >> choice;
    }

    switch (choice) {
    case 1:
        if (!station.AddRobot("KEEPER"))
            cout << "Ошибка! Кажется, у вас недостаточно средств...\n";
        else cout << "Новый робот успешно создан!\n";
        break;
    case 2:
        if (!station.AddRobot("INTEGRATOR"))
            cout << "Ошибка! Кажется, у вас недостаточно средств...\n";
        else cout << "Новый робот успешно создан!\n";
        break;
    case 3:
        if (!station.AddRobot("OPTY"))
            cout << "Ошибка! Кажется, у вас недостаточно средств...\n";
        else cout << "Новый робот успешно создан!\n";
        break;
    }
}

void getChoiceBuildModuls(Station& station) {
    cout << "                      [СОЗДАНИЕ МОДУЛЕЙ]\n";
    cout << "1. Архив\n";
    cout << "2. Командный отсек\n";
    cout << "3. Жилой отсек\n";
    cout << "4. Генераторная\n";
    cout << "Выберете желаемый модуль: ";
    cout.flush();

    int choice;
    cin >> choice;

    while ((choice < 1) || (choice > 4)) {
        cout << "Ошибка! Попробуйте ещё раз: \n";
        cout << "1. Архив\n2. Командный отсек\n3. Жилой отсек\n4. Генераторная\n";
        cout << "Выберете желаемый модуль: ";
        cout.flush();
        cin >> choice;
    }

    switch (choice) {
    case 1:
        if (!station.AddModule("Архив"))
            cout << "Ошибка! Кажется, у вас недостаточно средств...\n";
        else cout << "Архив успешно создан!\n";
        break;
    case 2:
        if (!station.AddModule("Командный отсек"))
            cout << "Ошибка! Кажется, у вас недостаточно средств...\n";
        else cout << "Командный отсек успешно создан!\n";
        break;
    case 3:
        if (!station.AddModule("Жилой отсек"))
            cout << "Ошибка! Кажется, у вас недостаточно средств...\n";
        else cout << "Жилой отсек успешно создан!\n";
        break;
    case 4:
        if (!station.AddModule("Генераторная"))
            cout << "Ошибка! Кажется, у вас недостаточно средств...\n";
        else cout << "Генераторная успешно создана!\n";
        break;
    }
}

void getChoiceUpgrateModuls(Station& station) {
    cout << "                     [УЛУЧШЕНИЕ МОДУЛЕЙ]\n";
    printModulsList(station, 1);
    cout << "\nВыберете желаемый модуль: ";
    cout.flush();

    int choice;
    cin >> choice;
    cout << "\n";

    while ((choice < 1) || (choice > (int)station.getModules().size())) {
        cout << "Ошибка! Попробуйте ещё раз: \n";
        cin >> choice;
    }

    Module* selectedModule = station.getModule(choice - 1);

    if (selectedModule->upgrade(station) == 0) {
        cout << "Ошибка! Кажется, у вас недостаточно средств...\n";
    } else {
        cout << selectedModule->getType() << " успешно улучшен до уровня "
            << selectedModule->getLevel() << "!\n";
    }
}

int getChoiceCrossModule(Station& station, string num) {

    cout << "Выберете модуль " << num << ": ";
    cout.flush();

    int choice;
    cin >> choice;

    while ((choice < 1) || (choice > (int)station.getModules().size())) {
        cout << "Ошибка! Попробуйте ещё раз: ";
        cin >> choice;
    }

    return choice; 
}

int getChoiceCrossRobot(Station& station, string num) {
    cout << "Выберете робота " << num << ": ";
    cout.flush();

    int choice;
    cin >> choice;

    while ((choice < 1) || (choice > (int)station.getRobots().size())) {
        cout << "Ошибка! Попробуйте ещё раз: ";
        cin >> choice;
    }

    return choice; 
}

void crossModule(Station& station) {
    cout << "                     [СКРЕЩИВАНИЕ МОДУЛЕЙ]\n";

    int choiceA = getChoiceCrossModule(station, "A");
    int choiceB = getChoiceCrossModule(station, "B");

    Module* moduleA = station.getModule(choiceA - 1);
    Module* moduleB = station.getModule(choiceB - 1);

    Module* preview = *moduleA + *moduleB;
    if (!preview) {
        cout << "Скрешивание не удалось!\n";
        return;
    }

    cout << "                  [ПРЕДВАРИТЕЛЬНЫЙ РЕЗУЛЬТАТ]\n";
    cout << preview;
    cout << "1. Продолжить\n";
    cout << "0. Отмена\n";
    cout << "Выбор > ";

    int choice;
    cin >> choice;

    while (choice < 0 || choice > 1) {
        cout << "Ошибка! Попробуйте ещё раз:\n";
        cout << "Выбор > ";
        cin >> choice;
    }

    if (choice == 1) {
        station.addModuleDirect(preview);
        cout << "Скрещивание прошло успешно!\n";

        station.removeModule(choiceA - 1);
        if (choiceB > choiceA) station.removeModule(choiceB - 2);
        else station.removeModule(choiceB - 1);
    } else {
        delete preview;
    }
}

void crossRobot(Station& station) {
    cout << "                     [СКРЕЩИВАНИЕ РОБОТОВ]\n";

    int choiceA = getChoiceCrossRobot(station, "A");
    int choiceB = getChoiceCrossRobot(station, "B");

    if (choiceA == choiceB) {
        cout << "Нельзя скрещивать робота с самим собой!\n";
        return;
    }

    Robot* robotA = station.getRobot(choiceA - 1);
    Robot* robotB = station.getRobot(choiceB - 1);

    if (robotA->getAge() < 5 || robotB->getAge() < 5) {
        cout << "Оба робота должны быть старше 5 дней!\n";
        return;
    }

    Robot* preview = *robotA + *robotB;
    if (!preview) {
        cout << "Синтез не удался!\n";
        return;
    }

    cout << "                  [ПРЕДВАРИТЕЛЬНЫЙ РЕЗУЛЬТАТ]\n";
    cout << "Мама: " << robotA;
    cout << "Папа: " << robotB;
    cout << "Детё: " << preview;
    cout << "\n";
    cout << "1. Продолжить\n";
    cout << "0. Отмена\n";
    cout << "Выбор > ";

    int choice;
    cin >> choice;

    while (choice < 0 || choice > 1) {
        cout << "Ошибка! Попробуйте ещё раз:\n";
        cout << "Выбор > ";
        cin >> choice;
    }

    if (choice == 1) {
        station.addRobotDirect(preview);
        cout << "Синтез успешен! Создан робот: " << preview->getName() << "\n";
        cout << "Тип: " << preview->getType() << " | Родители: " << preview->getMom() << " и " << preview->getDad() << "\n";

        station.removeRobot(choiceA - 1);
        if (choiceB > choiceA) station.removeRobot(choiceB - 2);
        else station.removeRobot(choiceB - 1);
    } else {
        delete preview;
    }
}

void appointmentRobots(Station& station, string nameStation) {
    clearScreen();
    printStation(station, nameStation);
    printMoney(station);
    printModuls(station);

    cout << "                     [НАЗНАЧЕНИЕ РАБОЧИХ]\n";
    cout << "Выберите модуль: ";
    int choiceM;
    cin >> choiceM;
    
    while ((choiceM < 1) || (choiceM > (int)station.getModules().size())) {
        cout << "Ошибка! Попробуйте ещё раз: ";
        cin >> choiceM;
    }
    
    Module* module = station.getModule(choiceM - 1);

    clearScreen();
    printStation(station, nameStation);
    printMoney(station);
    printRobots(station);

    cout << "                     [НАЗНАЧЕНИЕ РАБОЧИХ]\n";
    cout << "Выберите рабочего: ";
    int choiceR;
    cin >> choiceR;
    
    while ((choiceR < 1) || (choiceR > (int)station.getRobots().size())) {
        cout << "Ошибка! Попробуйте ещё раз: ";
        cin >> choiceR;
    }

    Robot* robot = station.getRobot(choiceR - 1);

    clearScreen();
    printStation(station, nameStation);
    printMoney(station);

    cout << "                     [НАЗНАЧЕНИЕ РАБОЧИХ]\n";
    cout << "Выбранный модуль: " << module;
    cout << "Выбранный рабочий: " << robot;
    cout << "Выберите действие:\n";
    cout << "1. Назначить на должность\n";
    cout << "2. Снять с должности\n";
    cout << "Выбор > ";

    int choiceTie;
    cin >> choiceTie;

    while ((choiceTie < 1) || (choiceTie > 2)) {
        cout << "Ошибка! Попробуйте ещё раз: ";
        cout << "Выбор > ";
        cin >> choiceTie;
    }

    if (choiceTie == 1) {
        *module += robot;
    } else *module -= robot;
}

void modulsMenu(Station& station, string nameStation) {
    int stop = 0;
    int choice;

    while (!stop) {
        clearScreen();
        printStation(station, nameStation);
        printMoney(station);
        printModuls(station);
        printRobots(station);

        int stopMenu = 0;
        while (!stopMenu) {
            cout << "   ┌────────────────МЕНЮ УПРАВЛЕНИЯ МОДУЛЯМИ────────────────┐\n";
            cout << "   | 1. Управление модулями (Вкл/Выкл)                      |\n";
            cout << "   | 2. Построить модуль                                    |\n";
            cout << "   | 3. Улучшить модуль                                     |\n";
            cout << "   | 4. Список всех модулей                                 |\n";
            cout << "   | 5. Модульная сборка                                    |\n";
            cout << "   | 0. Назад                                               |\n";
            cout << "   └────────────────────────────────────────────────────────┘\n";

            cout << "Введите команду > ";
            cout.flush();
            choice = getChoice('5');
            cout << "\n";

            switch (choice) {
            case '1':
                clearScreen();
                printStation(station, nameStation);
                printMoney(station);
                printModuls(station);
                getChoiceModuls(station);
                break;
            case '2':
                clearScreen();
                printStation(station, nameStation);
                printMoney(station);
                printModuls(station);
                getChoiceBuildModuls(station);
                break;
            case '3':
                clearScreen();
                printStation(station, nameStation);
                printMoney(station);
                getChoiceUpgrateModuls(station);
                break;
            case '4':
                clearScreen();
                printStation(station, nameStation);
                printMoney(station);
                printModuls(station);
                break;
            case '5':
                clearScreen();
                printStation(station, nameStation);
                printMoney(station);
                printModuls(station);
                crossModule(station);
                break;
            case '0':
                stopMenu = 1;
                stop = 1;
                clearScreen();
                printStation(station, nameStation);
                printMoney(station);
                printModuls(station);
                printRobots(station);
                break;
            default:
                break;
            }

            if (stopMenu == 1) continue;
        }
        if (stop == 1) continue;
    }
}

void robotsMenu(Station& station, std::string nameStation) {
    int stop = 0;
    int choice;

    while (!stop) {
        clearScreen();
        printStation(station, nameStation);
        printMoney(station);
        printModuls(station);
        printRobots(station);

        int stopMenu = 0;
        while (!stopMenu) {
            cout << "   ┌────────────────МЕНЮ УПРАВЛЕНИЯ РОБОТАМИ────────────────┐\n";
            cout << "   | 1. Создать нового робота (Цена: 150 энергии 200 битов) |\n";
            cout << "   | 2. Список всех роботов                                 |\n";
            cout << "   | 3. Синтез сознания                                     |\n";
            cout << "   | 4. Назначение на смену                                 |\n";
            cout << "   | 0. Назад                                               |\n";
            cout << "   └────────────────────────────────────────────────────────┘\n";

            cout << "Введите команду > ";
            cout.flush();
            choice = getChoice('4');
            cout << "\n";

            switch (choice) {
            case '1':
                clearScreen();
                printStation(station, nameStation);
                printMoney(station);
                printRobots(station);
                utilsAddRobot(station);
                break;
            case '2':
                clearScreen();
                printStation(station, nameStation);
                printMoney(station);
                printRobots(station);
                break;
            case '3':
                clearScreen();
                printStation(station, nameStation);
                printMoney(station);
                printRobots(station);
                crossRobot(station);
                break;
            case '4':
                clearScreen();
                printStation(station, nameStation);
                printMoney(station);
                printModuls(station);
                printRobots(station);
                appointmentRobots(station, nameStation);
                break;
            case '0':
                stopMenu = 1;
                stop = 1;
                clearScreen();
                printStation(station, nameStation);
                printMoney(station);
                printModuls(station);
                printRobots(station);
                break;
            default:
                break;
            }

            if (stopMenu == 1) continue;
        }
        if (stop == 1) continue;
    }
}