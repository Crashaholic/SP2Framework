#ifndef GUIMANAGER_H
#define GUIMANAGER_H

#include "GUIScreen.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>

class GUIManager
{
public:
	static GUIManager* getInstance();
	~GUIManager();
	void renderUI();
	void renderText(std::string font, float xPos, float yPos, std::string text, float fontSize=1.0f, Color color=Color(1,1,1), TextAlignment align=TEXT_ALIGN_LEFT);
	unsigned int FBO;
	void update(double newX, double newY, double winWidth, double winHeight);
	void cursorUpdate(double newX, double newY, double winWidth, double winHeight);
	void buttonStateUpdate(double newX, double newY, double winWidth, double winHeight);
	void addButton(GUIButton* button);
private:
	void InitFBO();
	GUIManager();

	Cursor cursor;
	unsigned int VAO;
	unsigned int RBO;
	unsigned int textureVBO;
	unsigned int topdownTexture;
	static GUIManager* instance;
	std::vector<Texture*> textures;
	std::map<std::string, GUIFont*> fonts;
	std::vector<GUIText*> guiText;
	std::vector<IRender*> renderables;
	std::vector<GUIButton*> buttons;
};

#endif