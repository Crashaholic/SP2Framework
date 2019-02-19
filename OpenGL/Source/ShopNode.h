#ifndef SHOPNODE_H
#define SHOPNODE_H

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
	int moneySpent;

};

#endif