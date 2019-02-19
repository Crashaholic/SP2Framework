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
	//int Buy(/*Player,*/Car*&, int);
	void buyCar(Player *player, int carSelected);
	void buyUpgrade(Player *player);

	int Undo(Player *player);
	void exitShop();
private:
	shopNode* Head;
	shopNode* Tail;
};//make objMoney[4]
#endif