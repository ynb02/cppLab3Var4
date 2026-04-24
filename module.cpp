#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>

#include "module.h"
#include "game.h"
#include "utils.h"

using namespace std;

Module::Module() {
    isActive = true;
    level = 1;
}
Module::~Module() {}

string Module::tumb() {
    if (isActive == true) {
        isActive = false;
        return "отключён";
    } else {
        isActive = true;
        return "включён";
    }
}
bool Module::getActive() {
    return isActive;
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
    return 2 * level;
}
int Module::counterOpty(Station& station) {
    int counter = 0;
    for (auto& r : station.getRobots()) {
        if (r->getType() == "OPTY") counter += 1;
    }
    return counter * 5;
}
pair<int, int> Module::getUpgradePrice() {
    int first = 50 + 50 * level;
    int second = 100 + 100 * level;
    return {first, second};
}

Archive::Archive() : Module() {}
Archive::~Archive() {}
int Archive::GetEnergyInput(Station& station) {
    return 0 - 0 / 100 * counterOpty(station);
}

Center::Center() : Module() {}
Center::~Center() {}
int Center::GetEnergyInput(Station& station) {
    return 20 - 20 / 100 * counterOpty(station);
}

Live::Live() : Module() {}
Live::~Live() {}
int Live::GetEnergyInput(Station& station) {
    return 5 - 5 / 100 * counterOpty(station);
}

Generator::Generator() : Module() {}
Generator::~Generator() {}
int Generator::GetEnergyInput(Station& station) {
    return 0 - 0 / 100 * counterOpty(station);
}

ostream& operator<<(ostream& os, Module* m) {
    string t = "\n";
    string active;

    if (m->getActive() == 1) active = "ВКЛ";
    else active = "ВЫКЛ";

    if (m->getType() == "Жилой отсек") t = "";

    os << m->getType() << getSpace(15, (int)m->getType().length()) 
        << " (Lvl " << m->getLevel() << ") Статус: " << active << t;
    
    return os;
}

Module* Module::operator+(const Module& other) {

    if(getType() != other.getType()) return 0;

    if(getLevel() != other.getLevel()) return 0;

    Module* hamun = nullptr;
    if (getType() == "Архив") hamun = new Archive();
    else if (getType() == "Командный центр") hamun = new Center();
    else if (getType() == "Жилой отсек") hamun = new Live();
    else hamun = new Generator();

    hamun->level = getLevel() + 1;
    return hamun;
}

Module& Module::operator+=(Robot* robot) {
    for (auto r : workers) {
        if (r == robot) {
            cout << "Робот уже привязан к этому отсеку!\n";
            return *this;
        }
    }
    workers.push_back(robot);
    cout << "Робот успешно назначен на должность!\n";
    return *this;
}

Module& Module::operator-=(Robot* robot) {
    auto it = find(workers.begin(), workers.end(), robot);
    if (it != workers.end()) {
        workers.erase(it);
        cout << "Робот успешно отвязан с должности!\n";
    } else {
        cout << "Этот робот не привязан к отсеку!\n";
    }
    return *this;
}