#ifndef SHOP_H
#define SHOP_H
//#include "Player.h"
#include "Car.h"
class Shop {
public:
	Shop();
	~Shop();
	void enterShop();
	void Buy(int);
	void Undo(int);
	void exitShop();
private:
	shopNode* Head;
	shopNode* Tail;
};//make objMoney[4]


class shopNode {
public:
	shopNode();
	~shopNode();
	void setNext(shopNode*);
	void setBack(shopNode*);
	void setNext();
	void setBack();
	void setObjNo(int);
	shopNode* getNext();
	shopNode* getBack();
	int getObjNo();
protected:
	shopNode* Next;
	shopNode* Back;
	int objNo;//the obj bought eg. tires,engine,nitro
};
#endif