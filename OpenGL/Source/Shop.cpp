#include "Shop.h"
Shop::Shop(){
}
Shop::~Shop() {
}
void Shop::enterShop() {
	Head = Tail = new shopNode;
}
void Shop::Buy(int Number) {
	if (Number > 0 && Number <= 4) {
		//if (Player.getMoney() >= (Car.Tier[Number - 1] * objMoney[Number - 1])) {
		Tail->setObjNo(Number - 1);
		Tail->setNext();
		Tail = Tail->getNext();
		Tail->setBack(Tail->getBack);
		//Player.setMoney(Player.getMoney() - (Car.Tier[Number - 1] * "Money"));
		//++tier
	//}
			//else {
		Number = 0;
		//}
	}
	if (!Number) {
		//RenderTextOnScreen(meshList[GEO_TEXT], "Not Enough", color(0, 1, 0), 2, 2, 2);
	}
	else {
		//RenderTextOnScreen(meshList[GEO_TEXT], "Item Bought", color(0, 1, 0), 2, 2, 2);
	}
};
void Shop::Undo(int Obj) {
	shopNode* Current = Head;
	while (Current->getNext != nullptr) {//No case needed for only 1 obj since a new shopNode needs to be declared first for 'Buy(Number)' to even run
		if (Current->getObjNo==Obj) {
			//Player.setMoney(Player.getMoney() + ((Car.Tier[Obj]-1) * objMoney[Obj]));
			//--tier
			if (Current == Head) {//Don't need make a case for tail since tail is always empty
				Head = Head->getNext();
			}
			else {
				Current->getBack()->setNext(Current->getNext());
				Current->getNext()->setBack(Current->getBack());
			}
			delete Current;
			return;
		}
		Current = Current->getNext();
	}
	//RenderTextOnScreen(meshList[GEO_TEXT], "No item of that type bought this time", color(0, 1, 0), 2, 2, 2);
}
void Shop::exitShop() {
	while (Head->getNext() != nullptr) {
		Head=Head->getNext();
		delete Head->getBack();
	}
	delete Head;
}


shopNode::shopNode() {
	Next = nullptr;
	Back = nullptr;
	objNo=0;
}
shopNode::~shopNode() {
}
void shopNode::setNext(shopNode* Current) {
	Next = Current;
}
void shopNode::setNext() {
	Next = new shopNode;
}
void shopNode::setBack(shopNode* Current) {
	Back = Current;
}
void shopNode::setBack() {
	Back = new shopNode;
}
void shopNode::setObjNo(int Number) {
	objNo = Number;
}
shopNode* shopNode::getNext() {
	return Next;
}
shopNode* shopNode::getBack(){
	return Back;
}
int shopNode::getObjNo() {
	return objNo;
}