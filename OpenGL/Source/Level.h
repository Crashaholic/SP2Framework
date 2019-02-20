#ifndef LEVEL_H
#define LEVEL_H

#include "GUIScreen.h"
#include "Mesh.h"
#include "Utility.h"
#include <MatrixStack.h>
#include "QuadTree.h"

class Level
{
public:
	Level(const char* levelPath);

	void Load(std::string path);
	void setScreen(std::string screen);

	void Render();
	void renderGui();
	void renderMesh();

	

private:

	QuadTree* tree;

	std::string currentScreen;
	std::map<std::string, GUIScreen*> screens;
	std::map<std::string, Mesh*> objects;
	MS modelStack, projectionStack, viewStack;
};

#endif