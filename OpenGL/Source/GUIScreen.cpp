#include "GUIScreen.h"
#include "Manager.h"

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

	for (int i = 0; i < (int) text.size(); i++)
		if (text[i] != nullptr)
			delete text[i];

	for (int i = 0; i < (int)buttons.size(); i++)
		if (buttons[i] != nullptr)
			delete buttons[i];
}

void GUIScreen::Render()
{
	for (int i = 0; i < (int) text.size(); i++) 
	{
		text[i]->render();
		delete text[i];
	}


	for (int i = renderables.size() - 1; i >= 0; i--) {
		renderables[i]->draw();
	}

	text.clear();
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

