#ifndef LEVEL_H
#define LEVEL_H

#include "GUIScreen.h"
#include "Mesh.h"
#include "Utility.h"
#include <MatrixStack.h>
#include "QuadTree.h"
#include "LightSource.h"

class Level
{
public:
	Level(const char* levelPath);
	~Level();

	void Load(std::string levelPath);
	void setScreen(std::string screen);
	void spawnObject(Mesh* m);

	void Render();
	void Update(double dt);
	void renderGUI();
	void renderObjects();

	Mesh* getObject(std::string name);
	std::vector<LightSource*>* getLightSources();
	std::map<std::string, Mesh*>* getObjects();
	QuadTree* getTree();
	GUIScreen* getScreen();
	std::string getScreenName();

private:

	void renderSkybox();
	void renderMesh(Mesh* mesh);
	Cursor* cursor;
	QuadTree* tree;
	std::string currentScreen;
	std::vector<LightSource*> lightSources;
	std::map<std::string, GUIScreen*> screens;
	std::map<std::string, Mesh*> objects;
	MS modelStack, projectionStack, viewStack;
};

#endif