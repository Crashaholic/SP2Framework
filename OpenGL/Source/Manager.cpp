#include "Manager.h"

Manager* Manager::instance = nullptr;

Manager::Manager()
{
	shaders["lit"] = new ShaderProgram("Shader//Texture.vert", "Shader//Blending.frag");
	shaders["text"] = new ShaderProgram("Shader//Text.vert", "Shader//Text.frag");
	shaders["overlay"] = new ShaderProgram("Shader//UI.vert", "Shader//UI.frag");
	for(int i = 0; i < 2; i++)
		lightSources.push_back(new LightSource());

	tree = new QuadTree(Vector3(0, 0, 0), Vector3(100, 0, 100));

}


Manager::~Manager()
{
	delete tree;

	for (auto const& object : objects)
		if (object.second != nullptr)
			delete object.second;

	for (auto const& shader : shaders)
		if (shader.second != nullptr)
			delete shader.second;

	for (int i = 0; i < (int)lightSources.size(); i++)
		delete lightSources[i];

}


Manager* Manager::getInstance()
{
	if (instance == nullptr)
		instance = new Manager();
	return instance;
}

Mesh* Manager::getObject(std::string name)
{
	return objects[name];
}

std::vector<LightSource*>* Manager::getLightSources() {
	return &lightSources;
}

std::map<std::string, Mesh*>* Manager::getObjects()
{
	return &objects;
}


ShaderProgram* Manager::getShader(std::string name)
{
	return shaders[name];
}

std::map<std::string, ShaderProgram*>* Manager::getShaders()
{
	return &shaders;
}

void Manager::spawnObject(Mesh* mesh)
{
	objects[mesh->name] = mesh;
	if (mesh->collisionEnabled)
		tree->Insert(mesh);
}


QuadTree* Manager::getTree() {
	return tree;
}