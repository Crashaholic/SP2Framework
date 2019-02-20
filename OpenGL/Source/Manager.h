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

#include <iostream>

class Manager
{
public:
	~Manager();
	static Manager* getInstance();

	void loadMap();
	void loadPlayerProgress();
	void savePlayerProgress();

	//Mesh* getObject(std::string name);
	//std::vector<LightSource*>* getLightSources();
	//std::map<std::string, Mesh*>* getObjects();
	Level* getLevel();

	ShaderProgram* getShader(std::string name);
	std::map<std::string, ShaderProgram*>* getShaders();

private:

	Manager();
	static Manager* instance;
	bool carOneUnlock;
	bool carTwoUnlock;
	bool carThreeUnlock;
	int money;

	std::string currentLevel;
	std::map<std::string, Level*> levels;
	std::map<std::string, Mesh*> objects;
	std::map<std::string, ShaderProgram*> shaders;
	bool startsWith(std::string input, std::string keyWord);
};

#endif