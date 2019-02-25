#include "GUIScreen.h"
#include "Manager.h"
#include "GUIManager.h"
#include "Player.h"

GUIScreen::GUIScreen(std::string name) {
	this->name = name;
	cursor = nullptr;
}

GUIScreen::GUIScreen() {

}

GUIScreen::~GUIScreen()
{
	for (int i = 0; i < (int)renderables.size(); i++)
		if (renderables[i] != nullptr)
			delete renderables[i];

	for (int i = 0; i < (int) texts.size(); i++)
		if (texts[i] != nullptr)
			delete texts[i];

	for (int i = 0; i < (int)buttons.size(); i++)
		if (buttons[i] != nullptr)
			delete buttons[i];
}

void GUIScreen::Render()
{

	if (Manager::getInstance()->getLevelName() == "game") {


		Player* player = dynamic_cast<Player*>(Manager::getInstance()->getLevel()->getObject("player"));

		if (Application::IsKeyPressed('E')) {

			if (!player->isInVehicle) {

				Mesh* shop = Manager::getInstance()->getLevel()->getObject("shopkeeper");
				if ((player->position - shop->position).Length() <= 3.0f) {
					if (name == "shop")
						Manager::getInstance()->getLevel()->setScreen("gameplay");
					else
						Manager::getInstance()->getLevel()->setScreen("shop");
				}

				Mesh* race = Manager::getInstance()->getLevel()->getObject("racenpc");
				if ((player->position - race->position).Length() <= 3.0f) {
					Manager::getInstance()->getLevel()->setScreen("playermode");
				}

			}
		}

	}

	for (int i = 0; i < (int) renderables.size(); i++){
		renderables[i]->draw();
	}

	for (int i = 0; i < (int) texts.size(); i++) 
	{
		texts[i]->render();
		delete texts[i];
	}

	texts.clear();

	

}

void GUIScreen::Update(double dt)	
{
	Manager* manager = Manager::getInstance();

	if (name == "ingame")
	{
		if (manager->getGameState() == RACE_IDLE){
			manager->setGameState(RACE_STARTING);
		}
	}

	if (cursor != nullptr)
	{
		if (cursor->updateVars(Application::mouse_x, Application::mouse_y, Application::winWidth, Application::winHeight, dt))
		{
			Level* level = manager->getLevel();

			for (int i = 0; i < (int)buttons.size(); i++)
			{
				if (buttons[i]->checkStatus(cursor->getX(), cursor->getY(), Application::winWidth, Application::winHeight))
				{
					if (name == "mainmenu")
					{

						if (buttons[i]->getAction() == "playgame")
						{
							level->setScreen("gameselection");
							/*Manager::getInstance()->setLevel("game");*/
						}

					}
					else if (name == "gameselection")
					{
						if (buttons[i]->getAction() == "newgame")
						{
							manager->setLevel("game");
						}
						else if (buttons[i]->getAction() == "loadgame")
						{

						}
					}
					else if (name == "shop") {
						if (buttons[i]->getAction() == "buycar")
						{
							manager->getLevel()->setScreen("carselection");
						}
						if (buttons[i]->getAction() == "upgradenitro")
						{   //buy upgrade and push it to history
							manager->getShop()->buyUpgrade(dynamic_cast<Player*>(manager->getLevel()->getObject("player")), 1);
							manager->getLevel()->setScreen("confirmation");
						}
						if (buttons[i]->getAction() == "upgradetire")
						{
							manager->getShop()->buyUpgrade(dynamic_cast<Player*>(manager->getLevel()->getObject("player")), 2);
							manager->getLevel()->setScreen("confirmation");
						}
						if (buttons[i]->getAction() == "upgradeengine")
						{
							manager->getShop()->buyUpgrade(dynamic_cast<Player*>(manager->getLevel()->getObject("player")), 3);
							manager->getLevel()->setScreen("confirmation");
						}
						if (buttons[i]->getAction() == "undo")
						{
							manager->getLevel()->setScreen("confirmationundo");
						}
					}
					else if (name == "confirmation")
					{
						if (buttons[i]->getAction() == "confirmpurchase")
						{
							manager->getLevel()->setScreen("shop");
						}
						else if (buttons[i]->getAction() == "denypurchase")
						{
							manager->getLevel()->setScreen("shop");
							manager->getShop()->Undo(dynamic_cast<Player*>(manager->getLevel()->getObject("player")));
						}
					}
					else if (name == "confirmationundo")
					{
						if (buttons[i]->getAction() == "confirmundo")
						{
							manager->getShop()->Undo(dynamic_cast<Player*>(manager->getLevel()->getObject("player")));
							manager->getLevel()->setScreen("shop");
						}
						else if (buttons[i]->getAction() == "denyundo")
						{
							manager->getLevel()->setScreen("shop");
						}
					}
					else if (name == "carselection")
					{
						if (buttons[i]->getAction() == "buycartwo")
						{
							if (dynamic_cast<Player*>(manager->getLevel()->getObject("player"))->getCarsUnlocked(2) == false)
							{
								manager->getShop()->buyCar(dynamic_cast<Player*>(manager->getLevel()->getObject("player")), 2);
								manager->getLevel()->setScreen("shop");
							}
							else
							{
								manager->getLevel()->setScreen("shop");
							}
						}
						else if (buttons[i]->getAction() == "buycarthree")
						{
							if (dynamic_cast<Player*>(manager->getLevel()->getObject("player"))->getCarsUnlocked(3) == false)
							{
								manager->getShop()->buyCar(dynamic_cast<Player*>(manager->getLevel()->getObject("player")), 3);
								manager->getLevel()->setScreen("shop");
							}
							else
							{
								manager->getLevel()->setScreen("shop");
							}
						}
						else if (buttons[i]->getAction() == "buycarfour")
						{
							if (dynamic_cast<Player*>(manager->getLevel()->getObject("player"))->getCarsUnlocked(4) == false)
							{
								manager->getShop()->buyCar(dynamic_cast<Player*>(manager->getLevel()->getObject("player")), 4);
								manager->getLevel()->setScreen("shop");
							}
							else
							{
								manager->getLevel()->setScreen("shop");
							}
						}
					}
					else if (name == "playermode")
					{

						if (buttons[i]->getAction() == "playsingleplayer")
						{

						}
						else if (buttons[i]->getAction() == "playmultiplayer")
						{
							std::cout << "works" << std::endl;
							level->setScreen("ingame");
							dynamic_cast<Player*>(level->getObject("player"))->setCar(dynamic_cast<Car*>(level->getObject("car")));
							dynamic_cast<Player*>(level->getObject("player2"))->setCar(dynamic_cast<Car*>(level->getObject("car2")));
						}
					}
					else if (name == "endgame") {

						if (buttons[i]->getAction() == "backtomainmenu") {
							manager->setLevel("pregame");
							manager->getLevel()->setScreen("mainmenu");
						}
					}
				}

				

				if (buttons[i]->isHover(cursor->getX(), cursor->getY()))
				{
					GUIText* text = nullptr;
					if (buttons[i]->getAction() == "playsingleplayer")
					{
						text = GUIManager::getInstance()->renderText("consolas", 497, 217, "Race against time", 0.4f, Color(1, 1, 1), TEXT_ALIGN_LEFT);
						level->getScreen()->addText(text);
					}
					else if (buttons[i]->getAction() == "playmultiplayer")
					{
						text = GUIManager::getInstance()->renderText("consolas", 497, 217, "Race against another player!", 0.4f, Color(1, 1, 1), TEXT_ALIGN_LEFT, 300);
						level->getScreen()->addText(text);
					}
				}
			}
		}
	}

	//if (Manager::getInstance()->getLevelName() == "game") {


	//	Player* player = dynamic_cast<Player*>(Manager::getInstance()->getLevel()->getObject("player"));

	//	if (!player->isInVehicle && Application::IsKeyPressed('E')) {
	//		Mesh* shop = Manager::getInstance()->getLevel()->getObject("shopkeeper");
	//		if ((player->position - shop->position).Length() <= 3.0f) {
	//			Manager::getInstance()->getLevel()->setScreen("shop");
	//		}

	//		Mesh* race = Manager::getInstance()->getLevel()->getObject("racenpc");
	//		if ((player->position - race->position).Length() <= 3.0f) {
	//			Manager::getInstance()->getLevel()->setScreen("playermode");
	//		}
	//	}

	//}



}

IRender* GUIScreen::getItem(std::string name) {
	for (int i = 0; i < renderables.size(); i++)
	{
		if (renderables[i]->getName() == name)
		{
			return renderables[i];
		}
	}
	return nullptr;
}

void GUIScreen::removeItem(std::string name)
{
	for (int i = 0; i < renderables.size(); i++)
	{
		if (renderables[i]->getName() == name)
		{
			renderables.erase(renderables.begin() + i);
			return;
		}
	}
}

void GUIScreen::addButton(GUIButton* button)
{
	renderables.push_back(button->getIRender());
	buttons.push_back(button);
}


void GUIScreen::addTexture(GUITexture* texture)
{
	renderables.push_back(texture->getIRender());
}

void GUIScreen::addText(GUIText* text)
{
	texts.push_back(text);
}


void GUIScreen::setCursor(Cursor* cursor)
{
	this->cursor = cursor;
	renderables.push_back(cursor->getGUITexture()->getIRender());
}