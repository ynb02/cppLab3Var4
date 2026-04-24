#pragma once

#include <string>
#include <utility>
#include <iostream>
#include <vector>

class Station;
class Robot;

class Module {
private:
    int level;
    bool isActive;
    std::vector<Robot*> workers;
public:
    Module();
    virtual ~Module();

    virtual int GetEnergyInput(Station& station) = 0;
    virtual int GetEnergyOutput() = 0;
    virtual int GetDataInput() = 0;
    virtual int GetDataOutput() = 0;
    virtual std::string getType() const = 0;

    Module& operator+=(Robot* robot);
    Module& operator-=(Robot* robot);

    const std::vector<Robot*>& getWorkers() const { return workers; }
    
    int getWorkersBonus() const { return workers.size() * 5; }
    int getLevel() const { return level; };
    void setLevel() { level++; };
    int upgrade(Station& station);
    int up();
    int counterOpty(Station& station);
    std::pair<int, int> getUpgradePrice();
    bool getActive();
    std::string tumb();

    friend std::ostream& operator<<(std::ostream& os, Module* m);
    Module* operator+(const Module& other);
};

class Archive : public Module {
public:
    Archive();
    ~Archive() override;

    int GetEnergyInput(Station& station) override;
    int GetEnergyOutput() override { return 0 + up() + getWorkersBonus(); }
    int GetDataInput() override { return 0; }
    int GetDataOutput() override { return 30 + up() + getWorkersBonus(); }
    std::string getType() const override { return "Архив"; }
};

class Center : public Module {
public:
    Center();
    ~Center() override;

    int GetEnergyInput(Station& station) override;
    int GetEnergyOutput() override { return 0 + up() + getWorkersBonus(); }
    int GetDataInput() override { return 0; }
    int GetDataOutput() override { return 0 + up() + getWorkersBonus(); }
    std::string getType() const override { return "Командный центр"; }
};

class Live : public Module {
public:
    Live();
    ~Live() override;

    int GetEnergyInput(Station& station) override;
    int GetEnergyOutput() override { return 0 + up() + getWorkersBonus(); }
    int GetDataInput() override { return 0; }
    int GetDataOutput() override { return 0 + up() + getWorkersBonus(); }
    std::string getType() const override { return "Жилой отсек"; }
};

class Generator : public Module {
public:
    Generator();
    ~Generator() override;

    int GetEnergyInput(Station& station) override;
    int GetEnergyOutput() override { return 40 + up() + getWorkersBonus(); }
    int GetDataInput() override { return 0; }
    int GetDataOutput() override { return 0 + up() + getWorkersBonus(); }
    std::string getType() const override { return "Генераторная"; }
};