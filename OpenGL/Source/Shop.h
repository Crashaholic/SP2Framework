#ifndef SHOP_H
#define SHOP_H
//#include "Player.h"
#include "Car.h"
#include "Player.h"
#include "ShopNode.h"
class Shop
{
public:
	Shop();
	~Shop();
	void enterShop();
	void buyCar(Player *player, int carSelected);
	void buyUpgrade(Player *player, int choice);
	void addUpgradeToTransactionHistory(int tier, char type, int moneySpent);
	void addCarToTransactionHistory(int carType, int moneySpent);

	void Undo(Player *player);
	void exitShop();
private:
	shopNode* head;
	shopNode* tail;
};
#endif