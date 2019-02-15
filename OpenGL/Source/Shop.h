#ifndef SHOP_H
#define SHOP_H
//#include "Player.h"
#include "Car.h"
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


class Shop {
public:
	Shop();
	~Shop();
	void enterShop();
	int Buy(/*Player,*/Car*&, int);
	int Undo(/*Player,*/Car*&);
	void exitShop();
private:
	shopNode* Head;
	shopNode* Tail;
};//make objMoney[4]
#endif