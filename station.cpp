#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>

#include "station.h"
#include "game.h"
#include "utils.h"

using namespace std;

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

    robots.push_back(new Integrator(getRandomName("INTEGRATOR")));
    robots.push_back(new Integrator(getRandomName("INTEGRATOR")));
    robots.push_back(new Keeper(getRandomName("KEEPER")));
    robots.push_back(new Keeper(getRandomName("KEEPER")));
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
        earnEnergy  += m->GetEnergyOutput();
        totalData   += m->GetDataOutput();
        earnData    += m->GetDataOutput();
    }
    for (auto r : robots) {
        totalEnergy += r->ProduceResources().first;
        earnEnergy  += r->ProduceResources().first;
        totalData   += r->ProduceResources().second;
        earnData    += r->ProduceResources().second;
    }

    for (auto r : robots) {
    r->incrementAge();
    }

    if (randomHundred() < CalculateSignalChance()) {
        int x = 100 + 10 * day;
        if (x > 500) x = 500;
        eventMessages.push_back("Пойман сигнал! Получено " + to_string(x) + " битов.");
        totalData += x;
        earnData  += x;
    }

    if ((day % 5) == 0) {
        int percent10 = (totalEnergy / 100) * 10;
        int percent20 = (totalData / 100) * 20;
        if (totalEnergy <= 0) {
            totalData -= percent20;
            eventMessages.push_back("Так как E = 0 орбитальная корпорация взяла с вас 20% налог в размере " + to_string(percent20) + "B.");
        } else if ((totalEnergy <= 0) && (totalData <= 0)) {
            cout << "Орбитальная корпорация не нашла средств на налоги! Придётся вынужденно прервать игру...\n";
            return 0;
        } else {
            totalEnergy -= percent10;
            eventMessages.push_back("Орбитальная корпорация взяла с вас 10% налог в размере " + to_string(percent10) + "E.");
        }
    }

    int used = 0;
    for (auto r : robots) {
        if (r->getType() == "INTEGRATOR") used += 1;
        else used += 2;
        if (used > capacity()) for (int i = 0; i < 5; i++) (*r)--;
        else (*r)--;
    }

    for (auto m : modules) {
        if (!m->getActive()) continue;

        totalEnergy -= m->GetEnergyInput(*this);
        totalData   -= m->GetDataInput();
    }

    for (auto r : robots) {
        if (!r->needRepair(*this)) continue;
        else {
            (*r)++;
            eventMessages.push_back("Произведён плановый ремонт " + r->getName());
        }
    }

    for (auto it = robots.begin(); it != robots.end(); ) {
        if ((*it)->GetSost().first <= 0 || (*it)->GetSost().second <= 0) {
            eventMessages.push_back("Робот " + (*it)->getName() +
                " изношен до критического состояния и утилизирован.");
            delete *it;
            it = robots.erase(it);
        } else {
            ++it;
        }
    }

    if (totalEnergy <= 0) {
        for (auto& m : modules) {
            if (!m->getActive()) continue;
            if (m->getType() == "Жилой отсек") continue;

            m->tumb();
            eventMessages.push_back("Внимание! E < 0! Аварийное отключение " + m->getType() + "!");
        }
    }

    if ((totalEnergy <= 0) && (totalData <= 0)) {
        cout << "Кажется... всё?\n";
        return 0;
    }

    if (totalEnergy > 1000) {
        totalEnergy = 1000;
        eventMessages.push_back("Хранилище энергии переполнено! Остатки были выброшенны в космос...");
    }

    day++;
    return 1;
}

void Station::PrintEvents() const {
    cout << "                          [СОБЫТИЯ]\n";
    if (eventMessages.empty())
        cout << "• За прошедший день никаких особых событий не произошло.\n";
    else {
        for (const auto& msg : eventMessages) {
            cout << "• " << msg << "\n";
        }
    }
    cout << "• За предыдущий цикл получено " << earnEnergy << " энергии и " << earnData << " данных.\n";
}

void Station::ClearEvents() {
    eventMessages.clear();
}

int Station::used() {
    int used = 0;
    for (auto& r : robots) {
        if (r->getType() == "INTEGRATOR") used += 1;
        else used += 2;
    }
    return used;
}

int Station::capacity() {
    int capacity = 0;
    for (auto& m : modules) {
        if (!m->getActive()) continue;
        if (m->getType() == "Жилой отсек") capacity += 6 + 2 * m->getLevel();
    }
    return capacity;
}

bool Station::AddRobot(string type) {
    if ((totalEnergy < 150) || (totalData < 200)) return 0;

    if (type == "KEEPER") {
        robots.push_back(new Keeper(getRandomName("KEEPER")));
    } else if (type == "INTEGRATOR") {
        robots.push_back(new Integrator(getRandomName("INTEGRATOR")));
    } else {
        robots.push_back(new Opty(getRandomName("OPTY")));
    }

    totalEnergy -= 150;
    totalData   -= 200;
    return 1;
}

bool Station::AddModule(string type) {
    if ((totalEnergy < 150) || (totalData < 200)) return 0;

    if (type == "Архив") {
        modules.push_back(new Archive());
    } else if (type == "Командный отсек") {
        modules.push_back(new Center());
    } else if (type == "Жилой отсек") {
        modules.push_back(new Live());
    } else {
        modules.push_back(new Generator());
    }

    totalEnergy -= 150;
    totalData   -= 200;
    return 1;
}

void Station::removeRobot(int index) {
    Robot* r = robots[index];
    for (auto m : modules) *m -= r;
    delete r;
    robots.erase(robots.begin() + index);
}

void Station::removeModule(int index) {
    delete modules[index];
    modules.erase(modules.begin() + index);
}