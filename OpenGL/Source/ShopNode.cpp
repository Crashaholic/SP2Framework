#include "ShopNode.h"

shopNode::shopNode()
{
	Next = nullptr;
	Back = nullptr;
	upgradeTier = 1;
	upgradeType = 0;
	moneySpent = 0;
	carTypeBought = 0;
}

shopNode::~shopNode()
{

}

void shopNode::setNext(shopNode* Current)
{
	Next = Current;
}

//void shopNode::setNext()
//{
//	Next = new shopNode;
//}

void shopNode::setBack(shopNode* Current)
{
	Back = Current;
}

//void shopNode::setBack()
//{
//	Back = new shopNode;
//}

void shopNode::setUpgradeTier(int tier)
{
	upgradeTier = tier;
}

void shopNode::setUpgradeType(char type)
{
	upgradeType = type;
}
void shopNode::setCarBought(int type)
{
	carTypeBought = type;
}

void shopNode::setMoneySpent(int money)
{
	moneySpent = money;
}

shopNode* shopNode::getNext()
{
	return Next;
}

shopNode* shopNode::getBack()
{
	return Back;
}

int shopNode::getUpgradeTier()
{
	return upgradeTier;
}

char shopNode::getUpgradeType()
{
	return upgradeType;
}

int shopNode::getMoneySpent()
{
	return moneySpent;
}

int shopNode::getCarBought()
{
	return carTypeBought;
}