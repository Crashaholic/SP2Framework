#include "CarUpgrade.h"


CarUpgrade::CarUpgrade(std::string name, std::string category, int tier)
{
	this->name = name;
	nitro = engine = tyre = 0;
	setTier(category, tier);
}

CarUpgrade::CarUpgrade(std::string name, int nitro, int engine, int tyre) {
	this->name = name;
	this->nitro = nitro;
	this->engine = engine;
	this->tyre = tyre;
}

CarUpgrade::CarUpgrade(std::string name)
{
	this->name = name;
	setOne();
}


CarUpgrade::CarUpgrade()
{
}


CarUpgrade::~CarUpgrade()
{
}

void CarUpgrade::setOne()
{
	nitro = engine = tyre = 1;
}

void CarUpgrade::setTier(std::string cat, int id)
{
	if (cat == "nitro")
		nitro = id;
	else if (cat == "tyre")
		tyre = id;
	else if (cat == "engine")
		engine = id;
}

std::string CarUpgrade::getName()
{
	return name;
}

int CarUpgrade::getCost()
{
	return (nitro + engine + tyre) * 25;
}

int CarUpgrade::getTier(std::string cat)
{
	if (cat == "nitro")
		return nitro;
	else if (cat == "engine")
		return engine;
	else if (cat == "tyre")
		return tyre;
	return 0;
}

std::string CarUpgrade::getUpgradedCategory()
{

	if (nitro != 0)
		return "nitro";
	else if (engine != 0)
		return "engine";
	else if (tyre != 0)
		return "tyre";
}

