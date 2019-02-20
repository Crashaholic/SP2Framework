#include "GUIScreen.h"

GUIScreen::GUIScreen() {

}

GUIScreen::~GUIScreen()
{
	for (int i = 0; i < (int)renderables.size(); ++i)
		if (renderables[i] != nullptr)
			delete renderables[i];

	for (int i = 0; i < (int) text.size(); ++i)
		if (text[i] != nullptr)
			delete text[i];

	for (int i = 0; i < (int)buttons.size(); ++i)
		if (buttons[i] != nullptr)
			delete buttons[i];
}

void GUIScreen::render()
{
	for (int i = 0; i < (int) text.size(); i++) 
	{
		text[i]->render();
		delete text[i];
	}

	for (int i = 0; i < (int)buttons.size(); i++) 
	{
		buttons[i]->getIRender()->draw();
	}

	for (int i = 0; i < (int) renderables.size(); i++) 
	{
		renderables[i]->draw();
	}
	text.clear();
}


void GUIScreen::addButton(GUIButton* b)
{
	buttons.push_back(b);
}


