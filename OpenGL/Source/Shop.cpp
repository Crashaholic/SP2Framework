#include "Shop.h"
#include "CarUpgrade.h"

Shop::Shop()
{
	currentSelectedCar = 0;
}

Shop::~Shop()
{

}

void Shop::addItem(std::string name, int cost)
{
	costs[name] = cost;
}

void Shop::Buy(Player *player, std::string category)
{
	std::string name = carNames[currentSelectedCar];

	// Return cost of car if tier 1, if not return tier price
	float cost = (player->getUpgrade(name) == nullptr) ? costs[name] : (player->getUpgrade(name)->getTier(category)+1) * 25.0f;
	

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
		}
		// Upgrade
		else
		{
			upgrade->setTier(category, upgrade->getTier(category) + 1);
		}

		// Record purchase
		purchases.push_back(new CarUpgrade(name, category, 1));
	
	}
}

void Shop::Undo(Player *player)
{
	if (purchases.size() == 0) return;

	CarUpgrade* lastItem = purchases[purchases.size() - 1];
	if (lastItem != nullptr)
	{
		std::string cat = lastItem->getUpgradedCategory();
		int tier = lastItem->getTier(cat);
		CarUpgrade* upgrade = player->getUpgrade(lastItem->getName());

		// Undo the car purchase
		if (tier == 1)
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
			player->setMoney(player->getMoney() + lastItem->getCost());

			// Decrement tier from Player object
			upgrade->setTier(cat, --tier);
			
		}
		purchases.pop_back();
	}

}


int Shop::getCostOfCar(std::string name)
{
	return costs[name];
}

std::string Shop::getCar() {
	return carNames[currentSelectedCar];
}

void Shop::selectNextCar() {
	currentSelectedCar++;
	if (currentSelectedCar > carNames.size()) currentSelectedCar = 0;
}

void Shop::selectPrevCar() {
	currentSelectedCar--;
	if (currentSelectedCar < 0) currentSelectedCar = carNames.size() - 1;
}
