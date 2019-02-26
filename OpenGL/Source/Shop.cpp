#include "Shop.h"
#include "CarUpgrade.h"
#include "Manager.h"

Shop::Shop()
{
	currentSelectedCar = 0;
	displayCar = nullptr;
	rotateY = 15.0f;
}

Shop::~Shop()
{

}

void Shop::addItem(std::string name, int cost)
{
	carNames.push_back(name);
	costs[name] = cost;
	
}

void Shop::spawnDisplayCar() {
	if (displayCar == nullptr) {
		Level* level = Manager::getInstance()->getLevel();

		for (int i = 0; i < 2; i++) {
			//if (i >= 2) {
			//	cars.push_back(nullptr);
			//	continue;
			//}
			std::string modelPath = "Models//" + carNames[i] + ".obj";
			std::string texPath = "Models//" + carNames[i] + ".tga";
			cars.push_back(new Mesh("displayCar" + i, Primitives::loadModel(modelPath.c_str(), false), LoadTGA(texPath.c_str()), false, false));
			cars[i]->position.Set(0.0f, 1.62f, 104.315f);
			level->spawnObject(cars[i]);
			cars[i]->setVisible(false);
		}

		cars[0]->setVisible(true);

	}
}

void Shop::Update(double dt) {
	if(displayCar != nullptr)
		displayCar->rotation.y += dt * rotateY;
}

void Shop::BuyCar(Player* player, std::string car) {
	if (std::find(carNames.begin(), carNames.end(), car) == carNames.end()) return;
	float cost = costs[car];

	if (player->getMoney() >= cost) {
		player->setMoney(player->getMoney() - cost);
		player->getUpgrades()->push_back(new CarUpgrade(car));
		purchases.push_back(new CarUpgrade(car));
	}
	
	Manager::getInstance()->savePlayerProgress();
}

void Shop::Buy(Player *player, std::string category)
{
	std::string name = carNames[currentSelectedCar];

	float cost = (player->getUpgrade(name)->getTier(category)+1) * 25.0f;
	

	// If can afford, purchase
	if (player->getMoney() >= cost)
	{
		player->setMoney(player->getMoney() - cost);
		// Get existing upgrade from player
		CarUpgrade* upgrade = player->getUpgrade(name);

		// New purchase
		if (upgrade == nullptr)
		{
			std::vector<CarUpgrade*>* playerUpgrades = player->getUpgrades();
			CarUpgrade* pUpgrade = new CarUpgrade(name);
			pUpgrade->setOne();
			playerUpgrades->push_back(pUpgrade);
			purchases.push_back(new CarUpgrade(name, 1, 1, 1));
		}
		// Upgrade
		else
		{
			upgrade->setTier(category, upgrade->getTier(category) + 1);
			purchases.push_back(new CarUpgrade(name, category, 1));
		}



		
	
	}

	Manager::getInstance()->savePlayerProgress();
	//std::vector<CarUpgrade*>* upgrades = player->getUpgrades();
	//for (int i = 0; i < upgrades->size(); i++) {
	//	std::cout << upgrades->at(i)->getName() << ": N" << upgrades->at(i)->getTier("nitro")
	//		<< ", E" << upgrades->at(i)->getTier("engine") << ", T" << upgrades->at(i)->getTier("tyre") << std::endl;
	//}
}

void Shop::Undo(Player *player)
{
	if (purchases.size() == 0) return;

	CarUpgrade* lastItem = purchases[purchases.size() - 1];
	if (lastItem != nullptr)
	{
		std::string cat = lastItem->getUpgradedCategory();
		int tier = lastItem->getTier(cat);
		bool refundCar = (lastItem->getTier("nitro") == 1 && lastItem->getTier("engine") == 1 && lastItem->getTier("tyre") == 1);
		CarUpgrade* upgrade = player->getUpgrade(lastItem->getName());

		std::cout << cat << std::endl;
		std::cout << tier << std::endl;
		std::cout << upgrade << std::endl;

		// Undo the car purchase
		if (refundCar)
		{
			// Refund cost of car
			player->setMoney(player->getMoney() + costs[lastItem->getName()]);

			// Delete car upgrade from Player object
			std::vector<CarUpgrade*>* playerUpgrades = player->getUpgrades();
			playerUpgrades->erase(std::find(playerUpgrades->begin(), playerUpgrades->end(), upgrade));

		}
		// Undo the tier purchase
		else
		{
			// Refund cost of upgrade
			player->setMoney(player->getMoney() + 25 * upgrade->getTier(cat));

			// Decrement tier from Player object
			upgrade->setTier(cat, upgrade->getTier(cat) - 1);
			
		}
		purchases.pop_back();
	}

	Manager::getInstance()->savePlayerProgress();
}


int Shop::getCostOfCar(std::string name)
{
	return costs[name];
}

std::string Shop::getCar() {
	return carNames[currentSelectedCar];
}

void Shop::selectNextCar() {
	displayCar->setVisible(false);
	currentSelectedCar++;
	if (currentSelectedCar == carNames.size()) currentSelectedCar = 0;
	displayCar = cars[currentSelectedCar];
	if(displayCar != nullptr)
		displayCar->setVisible(true);

}

void Shop::selectPrevCar() {
	displayCar->setVisible(false);
	if (currentSelectedCar < 0) currentSelectedCar = carNames.size() - 1;
	displayCar = cars[currentSelectedCar];
	if (displayCar != nullptr)
		displayCar->setVisible(true);
}
