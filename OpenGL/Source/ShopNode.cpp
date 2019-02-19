#include "ShopNode.h"

shopNode::shopNode()
{
	Next = nullptr;
	Back = nullptr;
	objNo = 0;
}

shopNode::~shopNode()
{

}

void shopNode::setNext(shopNode* Current)
{
	Next = Current;
}

void shopNode::setNext()
{
	Next = new shopNode;
}

void shopNode::setBack(shopNode* Current)
{
	Back = Current;
}

void shopNode::setBack()
{
	Back = new shopNode;
}

void shopNode::setObjNo(int Number)
{
	objNo = Number;
}

shopNode* shopNode::getNext()
{
	return Next;
}

shopNode* shopNode::getBack()
{
	return Back;
}

int shopNode::getObjNo()
{
	return objNo;
}