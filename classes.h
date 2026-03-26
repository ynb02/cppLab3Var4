#pragma once

#include <string>
#include <utility>
#include <iostream>
#include <vector>

class Station;

class Module {
private:
	int level;
	bool isActive;
public:
	Module();
	virtual ~Module();

	virtual int GetEnergyInput() = 0;
	virtual int GetEnergyOutput() = 0;
	virtual int GetDataInput() = 0;
	virtual int GetDataOutput() = 0;
	virtual std::wstring getType() = 0;

	int getLevel() { return level; }
	int upgrade(Station& station);
	int up();
	bool getActive();
	std::wstring tumb();
};

class Archive : public Module {
public:
	Archive();
	~Archive() override;

	int GetEnergyInput() override { return 0; }
	int GetEnergyOutput() override { return 0 + up(); }
	int GetDataInput() override { return 0; }
	int GetDataOutput() override { return 30 + up(); }
	std::wstring getType() override { return L"Архив"; }
};
class Center : public Module {
public:
	Center();
	~Center() override;

	int GetEnergyInput() override { return 20; }
	int GetEnergyOutput() override { return 0 + up(); }
	int GetDataInput() override { return 0; }
	int GetDataOutput() override { return 0 + up(); }
	std::wstring getType() override { return L"Командный центр"; }
};
class Live : public Module {
public:
	Live();
	~Live() override;

	int GetEnergyInput() override { return 5; }
	int GetEnergyOutput() override { return 0 + up(); }
	int GetDataInput() override { return 0; } 
	int GetDataOutput() override { return 0 + up(); }
	std::wstring getType() override { return L"Жилой отсек"; }
};
class Generator : public Module {
public:
	Generator();
	~Generator() override;

	int GetEnergyInput() override { return 0; }
	int GetEnergyOutput() override { return 40 + up(); }
	int GetDataInput() override { return 0; }
	int GetDataOutput() override { return 0 + up(); }
	std::wstring getType() override { return L"Генераторная"; }
};

class Robot {
private:
	int ChassisIntegrity = 100;
	int FirmwareIntegrity = 100;
	bool isActive;
protected:
	std::wstring Name;
public:
	Robot(const std::wstring& name);
	virtual ~Robot();

	virtual std::pair<int, int> ProduceResources() = 0;
	virtual std::wstring getType() = 0;
	virtual std::wstring getName() const = 0;

	void BreakSost(int multiplier = 1);
	void tumb();
	bool getActive();
	bool RepairSost(Station& station);
	std::pair<int, int> GetSost() { return { ChassisIntegrity, FirmwareIntegrity }; }
};
class Integrator : public Robot {
public:
	Integrator(const std::wstring& name);
	~Integrator() override;

	std::pair<int, int> ProduceResources() override { return { 25,3 }; }
	std::wstring getType() override { return L"INTEGRATOR"; }
	std::wstring getName() const override { return Name; }
};
class Keeper : public Robot {
public:
	Keeper(const std::wstring& name);
	~Keeper() override;

	std::pair<int, int> ProduceResources() override { return { 5,15 }; }
	std::wstring getType() override { return L"KEEPER"; }
	std::wstring getName() const override { return Name; }
};
class Opty : public Robot {
public:
	Opty(const std::wstring& name);
	~Opty() override;

	std::pair<int, int> ProduceResources() override { return { 0,0 }; }
	std::wstring getType() override { return L"OPTY"; }
	std::wstring getName() const override { return Name; }
};

class Station {
private:
	std::vector<std::wstring> eventMessages;
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
	bool AddRobot(std::wstring type);
	bool AddModule(std::wstring type);
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
	int getDay() const { return day; }
};