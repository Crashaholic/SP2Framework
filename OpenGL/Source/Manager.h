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

enum Sounds
{
	BGM_MAIN,
	BGM_INGAME,
	BGM_RACE,
	SFX_ACCELERATE,
	SFX_STEERING,
	SFX_VICTORY
};

class Manager
{
public:
	~Manager();
	static Manager* getInstance();

	void Init();
	// Load & Save
	void loadPlayerProgress();
	void savePlayerProgress(std::string path);
	void loadCars();

	// Racing
	void setGameState(RACE_STATE state);
	RACE_STATE getGameState();

	void setGameType(RACE_TYPE type);
	RACE_TYPE getGameType();

	double getRaceStartCountdown();
	void updateStartCountdown(double dt);
	int getPlacement(std::string name);

	Level* getLevel();
	void setLevel(std::string name);
	std::string& getLevelName();

	Camera* getCamera();

	ShaderProgram* getShader(std::string name);
	std::map<std::string, ShaderProgram*>* getShaders();


	void setPlayMusic(Sounds sound, bool flag);
	bool isPlayingMusic(Sounds sound);

	void createNewGame();
	void reloadLevel(std::string name);
	void loadSaveFile(std::string name);
	std::string getSaveFilePath();
	Shop* getShop();


private:

	Manager();
	static Manager* instance;

	bool isNewGame;
	std::string currentSaveFile;

	bool soundPlaying[5];

	double raceStartCountdown;
	Camera* mainmenu;
	Shop* shop;
	RACE_STATE gameState;
	RACE_TYPE gameType;

	std::string currentLevel;
	std::map<std::string, Level*> levels;
	std::map<std::string, ShaderProgram*> shaders;
};

#endif