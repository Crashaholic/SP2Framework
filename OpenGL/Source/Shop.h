#ifndef SHOP_H
#define SHOP_H

#include "Car.h"
#include "Player.h"

class Shop
{
public:
	
	Shop();
	~Shop();

	int getCostOfCar(std::string name);
	void addItem(std::string name, int cost);
	void Buy(Player* player, std::string name, std::string category, int tier);
	void Undo(Player *player);

private:
	std::map<std::string, int> costs;
	std::vector<CarUpgrade*> purchases;
};
#endif