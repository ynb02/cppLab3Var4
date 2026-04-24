#pragma once

#include <string>
#include "station.h"

std::string inputStationName();
std::string getRandomName(std::string type);
std::string getSpace(int max, int min);

int getChoice(char num);
int randomHundred();
int sostSpace(int input);
int utf8_length(const std::string& s);

void printRobots(Station& station);
void printModuls(Station& station);
void printMoney(Station& station);
void printStation(Station& station, std::string nameStation);
void getChoiceModuls(Station& station);
void utilsAddRobot(Station& station);
void getChoiceBuildModuls(Station& station);
void getChoiceUpgrateModuls(Station& station);
void printModulsList(Station& station, bool status);
void crossRobot(Station& station);
void crossModule(Station& station);
void appointmentRobots(Station& station);
void clearScreen();
void modulsMenu(Station& station, std::string nameStation);
void robotsMenu(Station& station, std::string nameStation);