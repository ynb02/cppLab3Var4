#pragma once

#include <string>
#include <utility>
#include <iostream>
#include <vector>

class Station;

class Robot {
private:
    int ChassisIntegrity = 100;
    int FirmwareIntegrity = 100;
    int age = 0;
    bool isActive;
    std::string mom = "Нет";
    std::string dad = "Нет";
protected:
    std::string Name;
public:
    Robot(const std::string& name);
    virtual ~Robot();

    virtual std::pair<int, int> ProduceResources() = 0;
    virtual std::string getType() const = 0;
    virtual std::string getName() const = 0;
    std::string getMom() { return mom; };
    std::string getDad() { return dad; };

    void operator++();
    void operator--();
    void operator++(int);
    void operator--(int);
    void incrementAge() { age++; }
    int getAge() const { return age; }

    void BreakSost(int multiplier = 1);
    void tumb();
    void setMom(std::string momy) {mom = momy; };
    void setDad(std::string dady) {dad = dady; };
    bool getActive();
    bool needRepair(Station& station);
    std::pair<int, int> GetSost() const { return { ChassisIntegrity, FirmwareIntegrity }; };

    friend std::ostream& operator<<(std::ostream& os, Robot* r);
    Robot* operator+(const Robot& other);
};

class Integrator : public Robot {
public:
    Integrator(const std::string& name);
    ~Integrator() override;

    std::pair<int, int> ProduceResources() override { return { 25, 3 }; }
    std::string getType() const override { return "INTEGRATOR"; }
    std::string getName() const override { return Name; }
};

class Keeper : public Robot {
public:
    Keeper(const std::string& name);
    ~Keeper() override;

    std::pair<int, int> ProduceResources() override { return { 5, 15 }; }
    std::string getType() const override { return "KEEPER"; }
    std::string getName() const override { return Name; }
};

class Opty : public Robot {
public:
    Opty(const std::string& name);
    ~Opty() override;

    std::pair<int, int> ProduceResources() override { return { 0, 0 }; }
    std::string getType() const override { return "OPTY"; }
    std::string getName() const override { return Name; }
};