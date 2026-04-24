#include <iostream>
#include <string>
#include <utility>
#include <cstdlib>

#include "utils.h"
#include "robot.h"
#include "module.h"
#include "station.h"

using namespace std;

int Game() {
    int stop = 0;
    int choice;

    string nameStation = inputStationName();
    if (nameStation.empty()) return 0;

    Station station;
    station.Init();

    while (!stop) {
        if (!station.ProcessDay()) {
            stop = 1;
            continue;
        }
        clearScreen();
        printStation(station, nameStation);
        printMoney(station);
        printModuls(station);
        printRobots(station);
        station.PrintEvents();

        int stopMenu = 0;
        while (!stopMenu) {
            cout << "   ┌────────────────────МЕНЮ УПРАВЛЕНИЯ─────────────────────┐\n";
            cout << "   | 1. Следующий день (Списать ресурсы)                    |\n";
            cout << "   | 2. Модули                                              |\n";
            cout << "   | 3. Роботы                                              |\n";
            cout << "   | 0. Выход                                               |\n";
            cout << "   └────────────────────────────────────────────────────────┘\n";

            cout << "Введите команду > ";
            cout.flush();
            choice = getChoice('3');
            cout << "\n";

            switch (choice) {
            case '1':
                stopMenu = 1;
                break;
            case '2':
                modulsMenu(station, nameStation);
                break;
            case '3':
                robotsMenu(station, nameStation);
                break;
            case '0':
                clearScreen();
                cout << "Выполняется выход...\n\n";
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