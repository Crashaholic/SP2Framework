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

class Manager
{
public:
	~Manager();
	static Manager* getInstance();

	void loadPlayerProgress();
	void savePlayerProgress();

	Level* getLevel();
	void setLevel(std::string name);
	std::string& getLevelName();
	Camera* getCamera();
	ShaderProgram* getShader(std::string name);
	std::map<std::string, ShaderProgram*>* getShaders();

private:

	Manager();
	static Manager* instance;
	Camera* mainmenu;
	bool carOneUnlock;
	bool carTwoUnlock;
	bool carThreeUnlock;
	int money;

	std::string currentLevel;
	std::map<std::string, Level*> levels;
	std::map<std::string, ShaderProgram*> shaders;
};

#endif