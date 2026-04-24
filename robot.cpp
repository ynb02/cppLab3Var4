#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>

#include "robot.h"
#include "game.h"
#include "utils.h"

using namespace std;

Robot::Robot(const string& name) {
    Name = name;
    isActive = true;
}
Robot::~Robot() {}

void Robot::BreakSost(int multiplier) {
    ChassisIntegrity -= 3 * multiplier;
    FirmwareIntegrity -= 3;
}

void Robot::tumb() {
    isActive = !isActive;
}
bool Robot::getActive() {
    return isActive;
}

bool Robot::needRepair(Station& station) {
    bool repaired = false;

    if (ChassisIntegrity < 100) {
        int chassisCost = (100 - ChassisIntegrity) * 5;
        if (station.getTotalEnergy() >= chassisCost) {
            station.setTotalEnergy(chassisCost);
            repaired = true;
        }
    }
    if (FirmwareIntegrity < 100) {
        int firmwareCost = (100 - FirmwareIntegrity) * 3;
        if (station.getTotalData() >= firmwareCost) {
            station.setTotalData(firmwareCost);
            repaired = true;
        }
    }

    return repaired;
}

Integrator::Integrator(const string& name) : Robot(name) {}
Integrator::~Integrator() {}

Keeper::Keeper(const string& name) : Robot(name) {}
Keeper::~Keeper() {}

Opty::Opty(const string& name) : Robot(name) {}
Opty::~Opty() {}

ostream& operator<<(ostream& os, Robot* r) {
    string typeRobot = r->getType();
    string proc;
	if (r->GetSost().first >= 50) proc = "OK";
	else if ((r->GetSost().first >= 25) && (r->GetSost().first < 50)) proc = "WARN";
	else if (r->GetSost().first < 25) proc = "CRIT";

	string active;
	if (r->getActive() == 1) active = "Активен";
	else active = "Мёртв";

    os << typeRobot << getSpace(10, typeRobot.length()) << " | " <<
			r->getName() << getSpace(13, r->getName().length()) << " | [" << r->GetSost().first << "] " <<
			getSpace(3, sostSpace(r->GetSost().first)) << proc << getSpace(4, proc.length()) << " | [" <<
			r->GetSost().second << "]  " << getSpace(3, sostSpace(r->GetSost().second)) << " | " << active <<
            getSpace(7, active.length()) << " | " << r->getAge() << endl;
    return os;
}

void Robot::operator--() {
    BreakSost(1);
}

void Robot::operator++() {
    ChassisIntegrity = 100;
    FirmwareIntegrity = 100;
}

void Robot::operator--(int) {
    BreakSost(1);
}

void  Robot::operator++(int) {
    ChassisIntegrity = 100;
    FirmwareIntegrity = 100;
}

Robot* Robot::operator+(const Robot& other) {
    if (age < 5 || other.age < 5)
        return nullptr;

    float mutation = 0.8f + (rand() % 41) / 100.0f;
    int newChassis  = ((GetSost().first  + other.GetSost().first)  / 2) * mutation;
    int newFirmware = ((GetSost().second + other.GetSost().second) / 2) * mutation;

    newChassis  = max(0, min(100, newChassis));
    newFirmware = max(0, min(100, newFirmware));

    int myPower    = GetSost().first + GetSost().second;
    int otherPower = other.GetSost().first + other.GetSost().second;
    string strongerType = (myPower >= otherPower) ? getType() : other.getType();

    string childName = getRandomName(strongerType);
    Robot* child = nullptr;

    if (strongerType == "INTEGRATOR")
        child = new Integrator(childName);
    else if (strongerType == "KEEPER")
        child = new Keeper(childName);
    else
        child = new Opty(childName);

    child->ChassisIntegrity  = newChassis;
    child->FirmwareIntegrity = newFirmware;
    child->setMom(getName());
    child->setDad(other.getName());

    return child;
}