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

	GUIText* renderText(std::string font, float xPos, float yPos, std::string text, float fontSize=1.0f, Color color=Color(1,1,1), TextAlignment align=TEXT_ALIGN_LEFT, float maxWidth=1024);

	unsigned int FBO;

private:

	static GUIManager* instance;
	GUIManager();

	void InitFBO();

	unsigned int VAO;
	unsigned int RBO;
	unsigned int textureVBO;
	unsigned int topdownTexture;

	std::map<std::string, GUIFont*> fonts;
};

#endif