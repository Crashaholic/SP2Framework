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

enum RACE_STATE {
	RACE_IDLE,
	RACE_STARTING,
	RACE_STARTED,
	RACE_COMPLETED,
};

class Manager
{
public:
	~Manager();
	static Manager* getInstance();

	void loadPlayerProgress();
	void savePlayerProgress();

	void setGameState(RACE_STATE state);
	RACE_STATE getGameState();
	double getRaceStartCountdown();
	void updateStartCountdown(double dt);

	Level* getLevel();
	void setLevel(std::string name);
	std::string& getLevelName();

	Camera* getCamera();

	ShaderProgram* getShader(std::string name);
	std::map<std::string, ShaderProgram*>* getShaders();

private:

	Manager();
	static Manager* instance;
	double raceStartCountdown;
	Camera* mainmenu;
	
	RACE_STATE gameState;
	bool carOneUnlock;
	bool carTwoUnlock;
	bool carThreeUnlock;
	int money;

	std::string currentLevel;
	std::map<std::string, Level*> levels;
	std::map<std::string, ShaderProgram*> shaders;
};

#endif