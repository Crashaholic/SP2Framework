#include "GUIManager.h"
#include "Manager.h"
#include "GL\glew.h"
#include "Utility.h"
#include "LoadTGA.h"
#include "GUITexture.h"

GUIManager* GUIManager::instance = nullptr;

GUIManager::GUIManager()
{
	fonts["bahnschrift"] = new GUIFont("Fonts//bahnschrift.fnt", "Fonts//bahnschrift.tga");
	fonts[ "consolas"  ] = new GUIFont("Fonts//consolas.fnt"   , "Fonts//consolas.tga"   );
	fonts[  "default"  ] = new GUIFont("Fonts//default.fnt"    , "Fonts//default.tga"    );
	GUIButton* button1 = new GUIButton(
		Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(128.0f, 128.f, 128.f), 
		LoadTGA("Image//rock.tga"), LoadTGA("Image//water.tga"), 
		GUIButton::NAME1);
	buttons.push_back(button1);
	renderables.push_back(cursor.getGUITexture()->getIRender());
}


GUIManager::~GUIManager()
{
	for (auto const& font : fonts)
		if (font.second != nullptr)
			delete font.second;

	for (int i = 0; i < (int) renderables.size(); i++)
		if(renderables[i] != nullptr)
			delete renderables[i];
}

void GUIManager::InitFBO()
{

}

void GUIManager::update(double newX, double newY, double winWidth, double winHeight)
{
	cursorUpdate(newX, newY, winWidth, winHeight);
	buttonStateUpdate(cursor.getX(), cursor.getY(), winWidth, winHeight);
}

void GUIManager::cursorUpdate(double newX, double newY, double winWidth, double winHeight)
{
	cursor.updateVars(newX, newY, winWidth, winHeight);
	cursor.updateTexture();
}

void GUIManager::buttonStateUpdate(double newX, double newY, double winWidth, double winHeight)
{
	for (int i = 0; i < (int)buttons.size(); i++) {
		buttons[i]->checkStatus(newX, newY, winWidth, winHeight);
	}
}

GUIManager* GUIManager::getInstance()
{
	if (instance == nullptr)
		instance = new GUIManager();
	return instance;
}

void GUIManager::renderUI() 
{
	
	for (int i = 0; i < (int)guiText.size(); i++) {
		guiText[i]->render();
		delete guiText[i];
	}

	for (int i = 0; i < (int)buttons.size(); i++) {
		buttons[i]->getIRender()->draw();
	}

	for (int i = 0; i < (int)renderables.size(); i++) {
		renderables[i]->draw();
	}
	guiText.clear();

}

void GUIManager::renderText(std::string font, float xPos, float yPos, std::string text, float fontSize, Color color, TextAlignment align)
{
	std::vector<GUIGlyph> glyphs;
	fonts[font]->createDataFromText(glyphs, xPos, yPos, text, fontSize, align);
	guiText.push_back(new GUIText(glyphs, xPos, yPos, fonts[font]->getTexture(), fontSize, color));

}
