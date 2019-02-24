#include "GUIScreen.h"
#include "Manager.h"
#include "GUIManager.h"


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
						Manager::getInstance()->getLevel()->setScreen("playermode");
					}

				}
				else if (name == "playermode")
				{

					if (buttons[i]->getName() == "singleplayer")
					{

					}
					else if (buttons[i]->getName() == "multiplayer") 
					{
						Manager::getInstance()->setLevel("game");
					}
				}
				else if (name == "shop")
				{
					std::cout << "upgrade called" << std::endl;
					if (buttons[i]->getName() == "buycar")
					{
						Manager::getInstance()->getLevel()->setScreen("carselection");
					}
					if (buttons[i]->getName() == "upgradenitro")
					{   //buy upgrade and push it to history
						Manager::getInstance()->getShop()->buyUpgrade(dynamic_cast<Player*>(Manager::getInstance()->getLevel()->getObject("player")), 1);
						Manager::getInstance()->getLevel()->setScreen("confirmation");
					}
					if (buttons[i]->getName() == "upgradetire")
					{
						Manager::getInstance()->getShop()->buyUpgrade(dynamic_cast<Player*>(Manager::getInstance()->getLevel()->getObject("player")), 2);
						Manager::getInstance()->getLevel()->setScreen("confirmation");
					}
					if (buttons[i]->getName() == "upgradeengine")
					{
						Manager::getInstance()->getShop()->buyUpgrade(dynamic_cast<Player*>(Manager::getInstance()->getLevel()->getObject("player")), 3);
						Manager::getInstance()->getLevel()->setScreen("confirmation");
					}
					if (buttons[i]->getName() == "undo")
					{
						Manager::getInstance()->getLevel()->setScreen("confirmationundo");
					}
				}
				else if (name == "confirmation")
				{
					if (buttons[i]->getName() == "confirmpurchase")
					{
						Manager::getInstance()->getLevel()->setScreen("shop");
					}
					else if (buttons[i]->getName() == "denypurchase")
					{
						Manager::getInstance()->getLevel()->setScreen("shop");
						Manager::getInstance()->getShop()->Undo(dynamic_cast<Player*>(Manager::getInstance()->getLevel()->getObject("player")));
					}
				}
				else if (name == "confirmationundo")
				{
					if (buttons[i]->getName() == "confirmundo")
					{
						Manager::getInstance()->getShop()->Undo(dynamic_cast<Player*>(Manager::getInstance()->getLevel()->getObject("player")));
						Manager::getInstance()->getLevel()->setScreen("shop");
					}
					else if (buttons[i]->getName() == "denyundo")
					{
						Manager::getInstance()->getLevel()->setScreen("shop");
					}
				}
				else if (name == "carselection")
				{
					if (buttons[i]->getName() == "buycartwo")
					{
						if (dynamic_cast<Player*>(Manager::getInstance()->getLevel()->getObject("player"))->getCarsUnlocked(2) == false)
						{
							Manager::getInstance()->getShop()->buyCar(dynamic_cast<Player*>(Manager::getInstance()->getLevel()->getObject("player")), 2);
							Manager::getInstance()->getLevel()->setScreen("shop");
						}
						else
						{
							Manager::getInstance()->getLevel()->setScreen("shop");
						}
					}
					else if(buttons[i]->getName() == "buycarthree")
					{
						if (dynamic_cast<Player*>(Manager::getInstance()->getLevel()->getObject("player"))->getCarsUnlocked(3) == false)
						{
							Manager::getInstance()->getShop()->buyCar(dynamic_cast<Player*>(Manager::getInstance()->getLevel()->getObject("player")), 3);
							Manager::getInstance()->getLevel()->setScreen("shop");
						}
						else
						{
							Manager::getInstance()->getLevel()->setScreen("shop");
						}
					}
					else if (buttons[i]->getName() == "buycarfour")
					{
						if (dynamic_cast<Player*>(Manager::getInstance()->getLevel()->getObject("player"))->getCarsUnlocked(4) == false)
						{
							Manager::getInstance()->getShop()->buyCar(dynamic_cast<Player*>(Manager::getInstance()->getLevel()->getObject("player")), 4);
							Manager::getInstance()->getLevel()->setScreen("shop");
						}
						else
						{
							Manager::getInstance()->getLevel()->setScreen("shop");
						}
					}
				}
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