#ifndef SHOPNODE_H
#define SHOPNODE_H

class shopNode {
public:
	shopNode();
	~shopNode();

	void setNext(shopNode*);
	void setBack(shopNode*);
	//void setNext();

	void setUpgradeType(char type);
	void setUpgradeTier(int tier);
	void setCarBought(int type);
	void setMoneySpent(int money);
	shopNode* getNext();
	shopNode* getBack();
	int getUpgradeTier();
	char getUpgradeType();
	int getMoneySpent();
	int getCarBought();

protected:
	shopNode* Next;
	shopNode* Back;

	char upgradeType; //Type of upgrade
	int upgradeTier; //Tier of upgrade
	int carTypeBought;
	int moneySpent;

};

#endif