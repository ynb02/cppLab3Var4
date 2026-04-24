#include <iostream>
#include <ctime>
#include <string>

#include "game.h"

using namespace std;

int main() {
    srand(time(nullptr));

    while (1) {
        cout << "          ┌────────────────────────────────────────┐\n";
        cout << "          |          ┌──────────────────┐          |\n";
        cout << "          |          | DIGITALIZED-GAME |          |\n";
        cout << "          |          └──────────────────┘          |\n";
        cout << "          |                 [МЕНЮ]                 |\n";
        cout << "          |            [1] Начать игру             |\n";
        cout << "          |            [0] Выход                   |\n";
        cout << "          |                                        |\n";
        cout << "          └────────────────────────────────────────┘\n";

        cout << "Введите команду > ";
        cout.flush();

        string input;
        getline(cin, input);

        while (input != "0" && input != "1") {
            cout << "Неизвестная команда! Попробуйте ещё раз...\n";
            cout << "Введите команду > ";
            cout.flush();
            getline(cin, input);
        }

        switch (input[0]) {
        case '1':
            if (!Game()) {
                cout << "При запуске игры произошла ошибка! Возврат в меню...\n";
                break;
            }
            break;
        case '0':
            cout << "Выполняется выход...";
            return 0;
        default:
            break;
        }
    }
    return 0;
}