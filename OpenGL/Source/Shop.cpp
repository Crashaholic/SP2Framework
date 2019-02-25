#include "Shop.h"
Shop::Shop()
{
	head = nullptr;
	tail = nullptr;
}

Shop::~Shop()
{

}

void Shop::enterShop()
{
}


void Shop::buyCar(Player *player, int carSelected)
{
	if (carSelected == 2 && player->getMoney() >= 50)
	{
		player->unlockCar(carSelected);
		player->setMoney(-50);
		addCarToTransactionHistory(2, 50);
	}
	if (carSelected == 3 && player->getMoney() >= 100)
	{
		player->unlockCar(carSelected);
		player->setMoney(-100);
		addCarToTransactionHistory(3, 100);
	}
	if (carSelected == 4 && player->getMoney() >= 200)
	{
		player->unlockCar(carSelected);
		player->setMoney(-200);
		addCarToTransactionHistory(4, 200);
	}
}

void Shop::buyUpgrade(Player *player, int choice)
{
	int nitroTier = player->getCar()->getNitroTier();
	int tireTier = player->getCar()->getTireTier();
	int engineTier = player->getCar()->getEngineTier();

	//1 for nitro, 2 for tire, 3 for engine
	if (choice == 1)
	{
		if (nitroTier == 1 && player->getMoney() >= 50)
		{
			player->setMoney(-50);
			player->getCar()->setNitroTier(2);
			addUpgradeToTransactionHistory(2, 'N', 50);
		}
		else if (nitroTier == 2 && player->getMoney() >= 75)
		{
			player->setMoney(-75);
			player->getCar()->setNitroTier(3);
			addUpgradeToTransactionHistory(3, 'N', 75);
		}
	}
	if (choice == 2)
	{
		
		if (tireTier == 1 && player->getMoney() >= 50)
		{
			player->setMoney(-50);
			player->getCar()->setTireTier(2);
			addUpgradeToTransactionHistory(2, 'T', 50);
		}
		else if (tireTier == 2 && player->getMoney() >= 75)
		{
			player->setMoney(-75);
			player->getCar()->setTireTier(3);
			addUpgradeToTransactionHistory(3, 'N', 75);
		}
		
	}
	if (choice == 3 && player->getMoney() >= 75)
	{
	
		if (engineTier == 1 && player->getMoney() >= 50)
		{
			player->setMoney(-50);
			player->getCar()->setEngineTier(2);
			addUpgradeToTransactionHistory(2, 'E', 50);
		}
		else if (engineTier == 2 && player->getMoney() >= 75)
		{
			player->setMoney(-75);
			player->getCar()->setEngineTier(3);
			addUpgradeToTransactionHistory(3, 'E', 75);
		}
	}
}

void Shop::addUpgradeToTransactionHistory(int tier, char type, int moneySpent)
{
	//Type of purchases:N for nitro, T for Tire & E for engine
	shopNode* current = nullptr;
	//if Link list is empty
	if (head == nullptr)
	{
		head = new shopNode;
		tail = head;
		current = head;

		current->setMoneySpent(moneySpent);
		current->setUpgradeTier(tier);
		current->setUpgradeType(type);
	}
	else if (head != nullptr)
	{
		current = new shopNode;
		
		current->setMoneySpent(moneySpent);
		current->setUpgradeTier(tier);
		current->setUpgradeType(type);

		//Step 1: link the tail to the new node
		tail->setNext(current);
		//step 2: link the new node to the previous node
		current->setBack(tail);
		tail = current;
	}
}

void Shop::addCarToTransactionHistory(int carType, int moneySpent)
{
	shopNode* current = nullptr;
	//if Link list is empty
	if (head == nullptr)
	{
		head = new shopNode;
		tail = head;
		current = head;

		current->setMoneySpent(moneySpent);
		current->setCarBought(carType);
	}
	else
	{
		current = new shopNode;

		current->setMoneySpent(moneySpent);
		current->setCarBought(carType);
		
		//Step 1: link the tail to the new node
		tail->setNext(current);
		//step 2: link the new node to the previous node
		current->setBack(tail);
		tail = current;
	}
}

void Shop::Undo(Player *player)
{
	shopNode* current = tail;

	if (head != nullptr)
	{
		//check what kind of transaction was made
		if (tail->getCarBought() == 0)
		{
			player->setMoney(tail->getMoneySpent());
			if (tail->getUpgradeType() == 'N')
			{
				player->getCar()->setNitroTier(player->getCar()->getNitroTier() - 1);
			}
			else if (tail->getUpgradeType() == 'T')
			{
				player->getCar()->setTireTier(player->getCar()->getTireTier() - 1);
			}
			else if (tail->getUpgradeType() == 'E')
			{
				player->getCar()->setEngineTier(player->getCar()->getEngineTier() - 1);
			}

			if (head == tail) //if the list only has 1 node
			{
				delete current;
				head = nullptr;
				tail = nullptr;
			}
			else
			{
				tail = tail->getBack();
				delete current;
			}
		}
		else //If transaction made was upgrade ^ or if it was car 
		{
			player->setMoney(tail->getMoneySpent());
			if (tail->getCarBought() == 2)
			{
				player->lockCar(2);
			}
			if (tail->getCarBought() == 3)
			{
				player->lockCar(3);
			}
			if (tail->getCarBought() == 4)
			{
				player->lockCar(4);
			}

			if (head == tail) //if the list only has 1 node
			{
				delete current;
				head = nullptr;
				tail = nullptr;
			}
			else
			{
				tail = tail->getBack();
				delete current;
			}

		}
	}
}


void Shop::exitShop()
{
	while (tail->getBack() != nullptr) {
		tail = tail->getBack();
		delete tail->getNext();
	}
	delete head;
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