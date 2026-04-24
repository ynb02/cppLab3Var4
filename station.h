#pragma once

#include <string>
#include <utility>
#include <iostream>
#include <vector>

#include "module.h"
#include "robot.h"

class Station;

class Station {
private:
    std::vector<std::string> eventMessages;
    std::vector<Robot*> robots;
    std::vector<Module*> modules;

    int totalEnergy;
    int totalData;
    int day;
    int earnEnergy;
    int earnData;

public:
    Station();
    ~Station();

    const std::vector<Robot*>& getRobots() const { return robots; }
    const std::vector<Module*>& getModules() const { return modules; }
    Robot* getRobot(int index) { return robots[index]; }
    Module* getModule(int index) { return modules[index]; }

    bool ProcessDay();
    bool AddRobot(std::string type);
    bool AddModule(std::string type);
    void Init();
    void PrintEvents() const;
    void ClearEvents();
    int used();
    int capacity();
    double getTotalPower();
    double CalculateSignalChance();
    int getTotalEnergy() const { return totalEnergy; }
    int getTotalData() const { return totalData; }
    void setTotalEnergy(int value) { totalEnergy -= value; }
    void setTotalData(int value) { totalData -= value; }
    void addRobotDirect(Robot* r) { robots.push_back(r); }
    void addModuleDirect(Module* m) { modules.push_back(m); }
    void removeRobot(int index);
    void removeModule(int index);
    int getDay() const { return day; }
};