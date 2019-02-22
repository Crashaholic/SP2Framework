#include "GUIScreen.h"
#include "Manager.h"
#include "GUIManager.h"
#include "Player.h"

GUIScreen::GUIScreen(std::string name) {
	this->name = name;
	cursor = new Cursor();
	renderables.push_back(cursor->getGUITexture()->getIRender());
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

	for (int i = renderables.size() - 1; i >= 0; i--) {
		renderables[i]->draw();
	}

	for (int i = 0; i < (int) texts.size(); i++) 
	{
		texts[i]->render();
		delete texts[i];
	}

	texts.clear();

	

}

void GUIScreen::Update()
{


	if (cursor != nullptr)
	{
		cursor->updateVars(Application::mouse_x, Application::mouse_y, Application::winWidth, Application::winHeight);
		cursor->updateTexture();

		for (int i = 0; i < (int)buttons.size(); i++)
		{
			if (buttons[i]->checkStatus(cursor->getX(), cursor->getY(), Application::winWidth, Application::winHeight))
			{
				if (name == "mainmenu") {

					if (buttons[i]->getName() == "playgame")
					{
						Manager::getInstance()->setLevel("game");
					}

				}
				else if (name == "playermode") {

					if (buttons[i]->getName() == "playsingleplayer") {

					}
					else if (buttons[i]->getName() == "playmultiplayer") {
						Level* level = Manager::getInstance()->getLevel();
						std::cout << "works" << std::endl;
						Manager::getInstance()->getLevel()->setScreen("gameplay");
						dynamic_cast<Player*>(level->getObject("player"))->setCar(dynamic_cast<Car*>(level->getObject("car")));
						dynamic_cast<Player*>(level->getObject("player2"))->setCar(dynamic_cast<Car*>(level->getObject("car2")));
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

	if (Manager::getInstance()->getLevelName() == "game") {

		Mesh* shop = Manager::getInstance()->getLevel()->getObject("shopkeeper");
		Player* player = dynamic_cast<Player*>(Manager::getInstance()->getLevel()->getObject("player"));

		if (!player->isInVehicle) {

			if ((player->position - shop->position).Length() <= 3.0f) {
				GUIManager* gui = GUIManager::getInstance();
				texts.push_back(gui->renderText("default", Application::winWidth / 2.0f, Application::winHeight / 2.0f, "Press E to open shop", 0.5f, Color(0, 1, 0), TEXT_ALIGN_MIDDLE));
			}

			Mesh* race = Manager::getInstance()->getLevel()->getObject("racenpc");
			if ((player->position - race->position).Length() <= 3.0f) {
				GUIManager* gui = GUIManager::getInstance();
				texts.push_back(gui->renderText("default", Application::winWidth / 2.0f, Application::winHeight / 2.0f, "Press E to enter race", 0.5f, Color(1, 0.5, 0), TEXT_ALIGN_MIDDLE));
			}
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