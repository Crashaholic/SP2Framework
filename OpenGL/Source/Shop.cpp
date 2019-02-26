#include "Shop.h"
#include "CarUpgrade.h"

Shop::Shop()
{

}

Shop::~Shop()
{

}

void Shop::addItem(std::string name, int cost)
{
	costs[name] = cost;
}

void Shop::Buy(Player *player, std::string name, std::string category, int tier)
{
	// Return cost of car if tier 1, if not return tier price
	float cost = (tier == 1) ? costs[name] : tier * 25.0f;

	// If can afford, purchase
	if (player->getMoney() >= cost)
	{
		player->setMoney(player->getMoney() - cost);
		// Get existing upgrade from player
		CarUpgrade* upgrade = player->getUpgrade(category);


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
			CarUpgrade* pUpgrade = player->getUpgrade(name);
			pUpgrade->setTier(category, tier);
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



