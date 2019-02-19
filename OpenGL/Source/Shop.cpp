#include "Shop.h"
Shop::Shop()
{

}

Shop::~Shop()
{

}

void Shop::enterShop()
{
	//Head = Tail = new shopNode;
}

//int Shop::Buy(/*Player Player,*/Car*& Car, int Number)
//{//4 options: Car, Nitro, Tires, Engine
//	//if (Number == 1) {
//	//	//Make the data of the car save if buying new car, be cause of delete
//	//	//if (Player.getMoney() >= (Player.getCarPrice[Number - 1] * Player.getCarTeir[Number - 1])) {
//	//	delete Car;//if/switch statement for different cars
//	//	Car = new Car1;
//	//	Tail->setObjNo(Number);
//	//	Tail->setNext();
//	//	Tail->getNext()->setBack(Tail);
//	//	Tail = Tail->getNext();
//	//	//Player.setMoney(Player.getMoney() - (Player.getCarPrice[Number - 1] * Player.getCarTeir[Number - 1]));
//	//	//}
//	//	//else{
//	//		//Number = 0;
//	//	//}
//	//}
//	//else if (Number >= 2 && Number <= 4) {
//	//	//if (Player.getMoney() >= (Car->getTier(Number - 2) * Car->getObjMoney(Number - 1))) {//&& teir limit not reached
//	//	Tail->setObjNo(Number);
//	//	Tail->setNext();
//	//	Tail->getNext()->setBack(Tail);
//	//	Tail = Tail->getNext();
//	//	//Player.setMoney(Player.getMoney() - (Car->getTier(Number - 2) * Car->getObjMoney(Number - 1)));
//	//	Car->Upgrade(Number - 2);
//	//	//}
//	//	/*else {
//	//		Number = 0;//This means not enough money
//	//	}*/
//	//}
//	//else if (Number == 6) {
//	//	Number = Undo(Car);
//	//}
//	//else {
//	//	Number = 5;//This means item does not exists
//	//}
//	//return Number;
//	return 0;
//}

void Shop::buyCar(Player *player, int carSelected)
{
	if (carSelected == 2 && player->getMoney() >= 50)
	{
		player->unlockCar(carSelected);
	}
	if (carSelected == 3 && player->getMoney() >= 100)
	{
		player->unlockCar(carSelected);
	}
	if (carSelected == 4 && player->getMoney() >= 200)
	{
		player->unlockCar(carSelected);
	}
}

void Shop::buyUpgrade(Player *player)
{
	int nitroTier = player->getCar()->getNitroTier();
	int tireTier = player->getCar()->getTireTier();
	int engineTier = player->getCar()->getEngineTier();

	if (nitroTier == 1 || tireTier == 1 || engineTier == 1 && player->getMoney() >= 50)
	{
		player->setMoney(-50);
		if (nitroTier == 1)
		{
			player->getCar()->setNitroTier(2);
		}
		else if (tireTier == 1)
		{
			player->getCar()->setTireTier(2);
		}
		else if (engineTier == 1)
		{
			player->getCar()->setEngineTier(2);
		}
	}
	if (nitroTier == 2 || tireTier == 2 || engineTier == 2 && player->getMoney() >= 75)
	{
		player->setMoney(-75);
		if (nitroTier == 2)
		{
			player->getCar()->setNitroTier(3);
		}
		else if (tireTier == 2)
		{
			player->getCar()->setTireTier(3);
		}
		else if (engineTier == 2)
		{
			player->getCar()->setEngineTier(3);
		}
	}
}

int Shop::Undo(Player *player)
{
	//if (Tail != Head) {
	//	if (Tail->getBack()->getObjNo() == 1) {
	//		//Player.setMoney(Player.getMoney() + (Player.getCarPrice[Number - 1] * Player.getCarTeir[Number - 1]-1));
	//		delete Car;
	//		Car = new Car1;
	//		//load the files.
	//	}
	//	else {
	//		//Car->setTier(Car->getTier()-1);
	//		//Player.setMoney(Player.getMoney() + ((Car->getTier(Tail->getBack()->getTier()-1)) * Car->getObjMoney(Tail->getBack()->getObjNo())));
	//	}
	//	if (Tail->getBack() == Head) {
	//		delete Tail;
	//		Head = Tail = new shopNode;
	//	}
	//	else {
	//		Tail->setNext(Tail);
	//		Tail = Tail->getBack();
	//		Tail->setBack(Tail->getBack());
	//		delete Tail->getNext();
	//	}
	//}
	//else {
	//	return 5;
	//}
	//return 6;
	return 0;
}


void Shop::exitShop()
{
	//while (Tail->getBack() != nullptr) {
	//	Tail = Tail->getBack();
	//	delete Tail->getNext();
	//}
	//delete Head;
}


//int main() {//what i used to test
//	int Item;
//	int inGame;
//	Shop Shop;
//	Car* Car;
//	Car = new Car1;
//	do {//Game loop
//		std::cout << "Select action (1=Enter Shop,0=ExitGame):\n";
//		std::cin >> inGame;
//		if (inGame) {
//			Shop.enterShop();
//			while (true) {//Shop loop
//				std::cout << "1:New Car\n2:Stronger Nitro\n3:New Tires\n4:Stronger Engine\n5:Exit\n6:Undo Last Item\n";
//				std::cout << "enter item No.:\n";
//
//				std::cin >> Item;
//				if (Item == 5) {
//					Shop.exitShop();
//					break;
//				}
//				Item = Shop.Buy(Car, Item);
//				if (!Item) {
//					std::cout << "Not enough Money\n";
//				}
//				else if (Item == 5) {
//					std::cout << "Item selected does not exists\n";
//				}
//				else if (Item == 1) {
//					std::cout << "new car bought\n";
//				}
//				else if (Item == 6) {
//					std::cout << "Undo Successful\n";
//				}
//				else {
//					std::cout << "Item " << Item << " Bought\n" << std::endl;
//				}
//			}
//		}
//	} while (inGame);
//}