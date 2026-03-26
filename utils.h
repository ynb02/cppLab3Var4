#pragma once

#include <string>
#include "classes.h"

std::wstring inputStationName();
std::wstring getRandomName(std::wstring type);
std::wstring getSpace(int max, int min);

int getChoice();
int randomHundred();
int sostSpace(int input);

void printRobots(Station& station);
void printModuls(Station& station);
void printMoney(Station& station);
void printStation(Station& station, std::wstring nameStation);
void getChoiceModuls(Station& station);
void utilsAddRobot(Station& station);
void getChoiceBuildModuls(Station& station);
void getChoiceUpgrateModuls(Station& station);
void printModulsList(Station& station);