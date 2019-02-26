#ifndef LEVEL_H
#define LEVEL_H

#include "GUIScreen.h"
#include "Mesh.h"
#include "Utility.h"
#include <MatrixStack.h>
#include "QuadTree.h"
#include "LightSource.h"
#include "Waypoint.h"

struct Object {
	std::map<std::string, std::string> values;
	void Set(std::string key, std::string value)
	{
		values[key] = value;
	}

	std::string Get(std::string key)
	{
		if (values.find(key) == values.end()) return "invalid";
		return values[key];
	}
};

class Level
{
public:
	Level(const char* levelPath, std::vector<Waypoint*>* waypoints);
	~Level();

	void Load(std::string levelPath, std::vector<Waypoint*>* waypoints);
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
	int getTotalLaps();

private:

	void renderSkybox();
	void renderMesh(Mesh* mesh);
	int totalLaps;
	Cursor* cursor;
	QuadTree* tree;
	std::string currentScreen;
	std::vector<LightSource*> lightSources;
	std::map<std::string, GUIScreen*> screens;
	std::map<std::string, Mesh*> objects;
	MS modelStack, projectionStack, viewStack;
};

#endif