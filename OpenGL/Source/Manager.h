#ifndef MANAGER_H
#define MANAGER_H

#include "LightSource.h"
#include "ShaderProgram.h"
#include "Mesh.h"
#include "FPSCamera.h"
#include <map>
#include <fstream>
#include <string>

#include <iostream>

class Manager
{
public:
	~Manager();
	static Manager* getInstance();

	void spawnObject(Mesh* mesh);
	void loadPlayerProgress();
	void savePlayerProgress();

	Mesh* getObject(std::string name);
	ShaderProgram* getShader(std::string name);

	std::vector<LightSource*>* getLightSources();
	std::map<std::string, Mesh*>* getObjects();
	std::map<std::string, ShaderProgram*>* getShaders();
	FPSCamera* getCamera();
private:

	Manager();
	static Manager* instance;
	std::map<std::string, Mesh*> objects;
	std::map<std::string, ShaderProgram*> shaders;
	std::vector<LightSource*> lightSources;
	FPSCamera* camera;
	bool startsWith(std::string input, std::string keyWord);
};

#endif