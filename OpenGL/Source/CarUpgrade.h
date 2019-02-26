#ifndef CARUPGRADE_H
#define CARUPGRADE_H

#include <string>


class CarUpgrade
{
public:

	CarUpgrade(std::string name);
	CarUpgrade(std::string name, std::string category, int tier);
	CarUpgrade(std::string name, int nitro, int tyre, int engine);
	CarUpgrade();
	~CarUpgrade();

	void setOne();
	std::string getName();
	int getCost();
	void setTier(std::string cat, int id);
	int getTier(std::string category);
	std::string getUpgradedCategory();

private:

	std::string name;
	int nitro;
	int tyre;
	int engine;
};

#endif