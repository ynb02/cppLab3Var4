#include <iostream>
#include <conio.h>
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <ctime>

#include "game.h"

using namespace std;

int main() {

	(void)_setmode(_fileno(stdout), _O_U16TEXT);
	(void)_setmode(_fileno(stdin), _O_U16TEXT);
	(void)_setmode(_fileno(stderr), _O_U16TEXT);

	srand(time(nullptr));

	while (1) {
		wcout << L"          ┌────────────────────────────────────────┐\n";
		wcout << L"          |          ┌──────────────────┐          |\n";
		wcout << L"          |          | DIGITALIZED-GAME |          |\n";
		wcout << L"          |          └──────────────────┘          |\n";
		wcout << L"          |                 [МЕНЮ]                 |\n";
		wcout << L"          |            [1] Начать игру             |\n";
		wcout << L"          |            [0] Выход                   |\n";
		wcout << L"          |                                        |\n";
		wcout << L"          └────────────────────────────────────────┘\n";

		wcout << L"Введите команду > ";
		int choice = _getch();
		wcout << L"\n";

		switch (choice) {
		case '1':
			if (!Game()) {
				wcout << L"При запуске игры произошла ошибка! Возврат в меню...\n";
				break;
			}
			break;
		case '0':
			wcout << L"Выполняется выход...";
			return 0;
		default:
			break;
		}
	}
	return 0;
}