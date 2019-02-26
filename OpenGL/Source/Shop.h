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
	void spawnDisplayCar();
	void Update(double dt);

	void addItem(std::string name, int cost);
	void BuyCar(Player* player, std::string car);
	void Buy(Player* player, std::string category);
	void Undo(Player *player);

	void selectNextCar();
	void selectPrevCar();
	std::string getCar();
	

private:
	float rotateY;
	int currentSelectedCar;
	Mesh* displayCar;
	std::vector<Mesh*> cars;
	std::vector<std::string> carNames;
	std::map<std::string, int> costs;
	std::vector<CarUpgrade*> purchases;
};
#endif