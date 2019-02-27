#ifndef MANAGER_H
#define MANAGER_H

#include "LightSource.h"
#include "ShaderProgram.h"
#include "Mesh.h"
#include "FreeLookCamera.h"
#include "QuadTree.h"
#include <map>
#include <fstream>
#include <string>
#include "Level.h"
#include <experimental/filesystem>
#include <iostream>
#include "Waypoint.h"
#include "Shop.h"
#include <ctime>

enum RACE_STATE {
	RACE_IDLE,
	RACE_STARTING,
	RACE_STARTED,
	RACE_COMPLETED,
};

enum RACE_TYPE {
	RACE_SINGLEPLAYER,
	RACE_MULTIPLAYER,
};

class Manager
{
public:
	~Manager();
	static Manager* getInstance();


	// Load & Save
	void loadPlayerProgress();
	void savePlayerProgress();
	void loadCars();

	// Racing
	void setGameState(RACE_STATE state);
	RACE_STATE getGameState();

	void setGameType(RACE_TYPE type);
	RACE_TYPE getGameType();

	double getRaceStartCountdown();
	void updateStartCountdown(double dt);
	std::vector<Waypoint*>* getWaypoints();
	int getPlacement(std::string name);

	Level* getLevel();
	void setLevel(std::string name);
	std::string& getLevelName();

	Camera* getCamera();

	ShaderProgram* getShader(std::string name);
	std::map<std::string, ShaderProgram*>* getShaders();


	Shop* getShop();

private:

	Manager();
	static Manager* instance;

	double raceStartCountdown;
	Camera* mainmenu;
	Shop* shop;
	RACE_STATE gameState;
	RACE_TYPE gameType;

	std::string currentLevel;
	std::map<std::string, Level*> levels;
	std::map<std::string, ShaderProgram*> shaders;
	std::vector<Waypoint*> waypoints;
};

#endif