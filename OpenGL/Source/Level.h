#ifndef LEVEL_H
#define LEVEL_H

#include "GUIScreen.h"
#include "Mesh.h"
#include "Utility.h"
#include <MatrixStack.h>

class Level
{
public:
	Level(const char* levelPath);

	void renderGui();
	void renderMesh();
	void renderLevel();
	Mesh* getMeshes(std::string);

private:
	std::map<std::string, GUIScreen*> levelGui;
	std::map<std::string, Mesh*> levelMesh;
	MS modelStack, projectionStack, viewStack;
};

#endif