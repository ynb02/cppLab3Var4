#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "classes.h"
#include "game.h"
#include "utils.h"

using namespace std;

Module::Module() {
    isActive = true;
    level = 1;
}
Module::~Module() {};
wstring Module::tumb() {
    if (isActive == true) {
        isActive = false;
        return L"отключён";
    }
    else {
        isActive = true;
        return L"включён";
    }
}
bool Module::getActive() {
    if (isActive == true) return true;
    else return false;
}
int Module::upgrade(Station& station) {
	if ((station.getTotalEnergy() < getUpgradePrice().first) 
		|| (station.getTotalData() < getUpgradePrice().second)) return 0;
	if (level >= 10) return 0;
	else {
		level += 1;
		station.setTotalEnergy(getUpgradePrice().first);
		station.setTotalData(getUpgradePrice().second);
		return level;
	}
}
int Module::up() {
	int up = 2 * level;
	return up;
}
pair<int, int> Module::getUpgradePrice() {
	int first = 50 + 50 * level;
	int second = 100 + 100 * level;

	return {first, second};
}

Archive::Archive() : Module() {};
Archive::~Archive() {};

Center::Center() : Module() {};
Center::~Center() {};

Live::Live() : Module() {};
Live::~Live() {};

Generator::Generator() : Module() {};
Generator::~Generator() {};

Robot::Robot(const wstring& name) {
    Name = name;
    isActive = true;
}
Robot::~Robot() {};
void Robot::BreakSost(int multiplier) {
    ChassisIntegrity -= 3 * multiplier;
    FirmwareIntegrity -= 3;
}
void Robot::tumb() {
    if (isActive == true) isActive = false;
    else isActive = true;
}
bool Robot::getActive() {
    if (isActive == true) return true;
    else return false;
}
bool Robot::RepairSost(Station& station) {
    if (ChassisIntegrity == 100 && FirmwareIntegrity == 100)
        return true;

    bool repaired = false;

    if (ChassisIntegrity < 100) {
        int chassisCost = (100 - ChassisIntegrity) * 5;
        if (station.getTotalEnergy() >= chassisCost) {
			station.setTotalEnergy(chassisCost);
            ChassisIntegrity = 100;
            repaired = true;
        }
    }

    if (FirmwareIntegrity < 100) {
        int firmwareCost = (100 - FirmwareIntegrity) * 3;
        if (station.getTotalData() >= firmwareCost) {
			station.setTotalData(firmwareCost);
            FirmwareIntegrity = 100;
            repaired = true;
        }
    }

    return repaired;
}

Integrator::Integrator(const wstring& name) : Robot(name) {}
Integrator::~Integrator() {}

Keeper::Keeper(const wstring& name) : Robot(name) {}
Keeper::~Keeper() {}

Opty::Opty(const wstring& name) : Robot(name) {}
Opty::~Opty() {}

Station::Station() : totalEnergy(200), totalData(150), day(0), earnEnergy(0), earnData(0) {}

Station::~Station() {
    for (auto r : robots) delete r;
    for (auto m : modules) delete m;
}

void Station::Init() {
    modules.push_back(new Archive());
    modules.push_back(new Center());
    modules.push_back(new Live());
    modules.push_back(new Generator());

    robots.push_back(new Integrator(getRandomName(L"INTEGRATOR")));
    robots.push_back(new Integrator(getRandomName(L"INTEGRATOR")));
    robots.push_back(new Keeper(getRandomName(L"KEEPER")));
    robots.push_back(new Keeper(getRandomName(L"KEEPER")));
}
double Station::getTotalPower() {
    double TotalPower = 0;
    for (auto m : modules) {
        if (!m->getActive()) continue;

        TotalPower += m->GetEnergyOutput();
    }
    for (auto r : robots) {
        TotalPower += r->ProduceResources().first;
    }
    return TotalPower;
}
double Station::CalculateSignalChance() {
    double Interference = robots.size() * 50;
    double TotalPower = getTotalPower();

    if (TotalPower + Interference == 0) return 0;

    double chance = (TotalPower / (TotalPower + Interference)) * 100;

    return min(95.0, chance);
}
bool Station::ProcessDay() {

	if (day == 0) {
		day++;
		return true;
	}

	eventMessages.clear();

	earnEnergy = 0;
	earnData = 0;

	for (auto m : modules) {
		if (!m->getActive()) continue;

		totalEnergy += m->GetEnergyOutput();
		earnEnergy += m->GetEnergyOutput();
		totalData += m->GetDataOutput();
		earnData += m->GetDataOutput();
	}
	for (auto r : robots) {
		totalEnergy += r->ProduceResources().first;
		earnEnergy += r->ProduceResources().first;
		totalData += r->ProduceResources().second;
		earnData += r->ProduceResources().second;
	}

	if (randomHundred() < CalculateSignalChance()) {
		int x = 100 + 10 * day;
		if (x > 500) x = 500;
		eventMessages.push_back(L"Пойман сигнал! Получено " + to_wstring(x) + L" битов.");
		totalData += x;
		earnData += x;
	}

	if ((day % 5) == 0) {
		int percent10 = (totalEnergy / 100) * 10;
		int percent20 = (totalData / 100) * 20;
		if (totalEnergy <= 0) {
			totalData -= percent20;
			eventMessages.push_back(L"Так как E = 0 орбитальная корпорация взяла с вас 20% налог в размере " + to_wstring(percent20) + L"B.");
		}
		else if ((totalEnergy <= 0) && (totalData <= 0)) {
			wcout << L"Орбитальная корпорация не нашла средств на налоги! Придётся вынужденно прервать игру...\n";
			return 0;
		}
		else {
			totalEnergy -= percent10;
			eventMessages.push_back(L"Орбитальная корпорация взяла с вас 10% налог в размере " + to_wstring(percent10) + L"E.");
		}
	}

	int used = 0;
	for (auto r : robots) {
		if (r->getType() == L"INTEGRATOR") used += 1;
		else used += 2;
		if (used > capacity()) r->BreakSost(5);
		else r->BreakSost();
	}

	for (auto m : modules) {
		if (!m->getActive()) continue;

		totalEnergy -= m->GetEnergyInput();
		totalData -= m->GetDataInput();
	}

	for (auto r : robots) {
		if (!r->RepairSost(*this)) {
			break;
		}
	}

	for (auto it = robots.begin(); it != robots.end(); ) {
		if ((*it)->GetSost().first <= 0 || (*it)->GetSost().second <= 0) {
			eventMessages.push_back(L"Робот " + (*it)->getName() +
				L" изношен до критического состояния и утилизирован.");
			delete* it;
			it = robots.erase(it);
		}
		else {
			++it;
		}
	}

	if (totalEnergy <= 0) {
		for (auto& m : modules) {
			if (!m->getActive()) continue;
			if (m->getType() == L"Жилой отсек") continue;

			m->tumb();

			eventMessages.push_back(L"Внимание! E < 0! Аварийное отключение " + m->getType() + L"!");
		}
	}

	if ((totalEnergy <= 0) && (totalData <= 0)) {
		wcout << L"Кажется... всё?\n";

		return 0;
	}

	if (totalEnergy > 1000) {
		totalEnergy = 1000;
		eventMessages.push_back(L"Хранилище энергии переполнено! Остатки были выброшенны в космос...");
	}

	day++;

	return 1;
}
void Station::PrintEvents() const {
	wcout << L"                          [СОБЫТИЯ]\n";
	if (eventMessages.empty()) wcout << L"За прошедший день никаких особых событий не произошло.\n";
	else {
		for (const auto& msg : eventMessages) {
			wcout << L"• " << msg << endl;
		}
	}
	wcout << L"За предыдущий цикл получено " << earnEnergy << L" энергии и " << earnData << L" данных.\n";
}

void Station::ClearEvents() {
	eventMessages.clear();
}

int Station::used() {
	int used = 0;
	for (auto& r : robots) {
		if (r->getType() == L"INTEGRATOR") used += 1;
		else used += 2;
	}
	return used;
}
int Station::capacity() {
	int capacity = 0;
	for (auto& m : modules) {
		if (!m->getActive()) continue;

		if (m->getType() == L"Жилой отсек") capacity += 6 + 2 * m->getLevel();
	}
	return capacity;
}
bool Station::AddRobot(wstring type) {
	if (type == L"KEEPER") {
		if ((totalEnergy < 150) || (totalData < 200)) return 0;

		wstring r = getRandomName(L"KEEPER");
		robots.push_back(new Keeper(getRandomName(L"KEEPER")));

		totalEnergy -= 150;
		totalData -= 200;

		return 1;
	}
	else if (type == L"INTEGRATOR") {
		if ((totalEnergy < 150) || (totalData < 200)) return 0;

		wstring r = getRandomName(L"INTEGRATOR");
		robots.push_back(new Integrator(getRandomName(L"INTEGRATOR")));

		totalEnergy -= 150;
		totalData -= 200;

		return 1;
	}
	else {
		if ((totalEnergy < 150) || (totalData < 200)) return 0;

		wstring r = getRandomName(L"OPTY");
		robots.push_back(new Opty(getRandomName(L"OPTY")));

		totalEnergy -= 150;
		totalData -= 200;

		return 1;
	}
}
bool Station::AddModule(wstring type) {
	if (type == L"Архив") {
		if ((totalEnergy < 150) || (totalData < 200)) return 0;

		modules.push_back(new Archive());

		totalEnergy -= 150;
		totalData -= 200;

		return 1;
	}
	else if (type == L"Командный отсек") {
		if ((totalEnergy < 150) || (totalData < 200)) return 0;

		modules.push_back(new Center());

		totalEnergy -= 150;
		totalData -= 200;

		return 1;
	}
	else if (type == L"Жилой отсек") {
		if ((totalEnergy < 150) || (totalData < 200)) return 0;

		modules.push_back(new Live());

		totalEnergy -= 150;
		totalData -= 200;

		return 1;
	}
	else {
		if ((totalEnergy < 150) || (totalData < 200)) return 0;

		modules.push_back(new Generator());

		totalEnergy -= 150;
		totalData -= 200;

		return 1;
	}
}