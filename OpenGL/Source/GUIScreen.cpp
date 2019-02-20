#include "GUIScreen.h"

GUIScreen::~GUIScreen()
{
	for (int i = 0; i < (int)iRender.size(); ++i)
		if (iRender[i] != nullptr)
			delete iRender[i];

	for (int i = 0; i < (int)guiText.size(); ++i)
		if (guiText[i] != nullptr)
			delete guiText[i];

	for (int i = 0; i < (int)guiButton.size(); ++i)
		if (guiButton[i] != nullptr)
			delete guiButton[i];
}

void GUIScreen::render()
{
	for (int i = 0; i < (int)guiText.size(); i++) 
	{
		guiText[i]->render();
		delete guiText[i];
	}

	for (int i = 0; i < (int)guiButton.size(); i++) 
	{
		guiButton[i]->getIRender()->draw();
	}

	for (int i = 0; i < (int)iRender.size(); i++) 
	{
		iRender[i]->draw();
	}
	guiText.clear();
}

void GUIScreen::pushAsRender(IRender* r)
{
	this->iRender.push_back(r);
}

void GUIScreen::pushAsText(GUIText* t)
{
	this->guiText.push_back(t);
}

void GUIScreen::pushAsButton(GUIButton* b)
{
	this->guiButton.push_back(b);
}