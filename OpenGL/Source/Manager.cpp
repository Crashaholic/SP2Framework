#include "Manager.h"

Manager* Manager::instance = nullptr;

Manager::Manager()
{
	shaders["lit"] = new ShaderProgram("Shader//Texture.vert", "Shader//Blending.frag");
	shaders["text"] = new ShaderProgram("Shader//Text.vert", "Shader//Text.frag");
	shaders["overlay"] = new ShaderProgram("Shader//UI.vert", "Shader//UI.frag");
	for(int i = 0; i < 2; i++)
		lightSources.push_back(new LightSource());
	camera = new FPSCamera();
	camera->Init(Vector3(0, 5, 0));
}


Manager::~Manager()
{
	for (auto const& object : objects)
		if (object.second != nullptr)
			delete object.second;

	for (auto const& shader : shaders)
		if (shader.second != nullptr)
			delete shader.second;

	for (int i = 0; i < (int)lightSources.size(); i++)
		delete lightSources[i];

	delete camera;
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
}

void Manager::loadPlayerProgress()
{
	std::fstream playerProgress; //input
	std::string line;

	playerProgress.open("playerProgress.txt", std::fstream::in);
	if (playerProgress.is_open())
	{
		while (getline(playerProgress, line))
		{
			std::vector<std::string> args = Utility::splitLine(line, '=');
			if (startsWith(line, "ID"))
			{
				/*std::cout << "ID has been secured";*/
				if (startsWith(args[1], "1"))
				{
					/*std::cout << "Car 1 has been unlocked";*/
				}
			}

			if (startsWith(line, "Upgrade"))
			{
				std::cout << "Car has been upgraded \n";
				std::vector<std::string> upgradeArgs = Utility::splitLine(args[1], ',');
				if (startsWith(upgradeArgs[0], "Nitro"))
				{
					std::vector<std::string> upgradeArgsStats = Utility::splitLine(upgradeArgs[0], ':');
					if (startsWith(upgradeArgsStats[1], "1"))
					{
						std::cout << "Nitro has been upgraded by 1\n";
					}
					else if (startsWith(upgradeArgsStats[1], "2"))
					{
						std::cout << "Nitro has been upgraded by 2\n";
					}
					else if (startsWith(upgradeArgsStats[1], "3"))
					{
						std::cout << "Nitro has been upgraded by 3\n";
					}
				}
				if (startsWith(upgradeArgs[1], "Tire"))
				{
					std::vector<std::string> upgradeArgsStats = Utility::splitLine(upgradeArgs[0], ':');
					if (startsWith(upgradeArgsStats[1], "1"))
					{
						std::cout << "Tire has been upgraded by 1\n";
					}
					else if (startsWith(upgradeArgsStats[1], "2"))
					{
						std::cout << "Tire has been upgraded by 2\n";
					}
					else if (startsWith(upgradeArgsStats[1], "3"))
					{
						std::cout << "Tire has been upgraded by 3\n";
					}
				}
				if (startsWith(upgradeArgs[2], "Engine"))
				{
					std::vector<std::string> upgradeArgsStats = Utility::splitLine(upgradeArgs[0], ':');
					if (startsWith(upgradeArgsStats[1], "1"))
					{
						std::cout << "Engine has been upgraded by 1\n";
					}
					else if (startsWith(upgradeArgsStats[1], "2"))
					{
						std::cout << "Engine has been upgraded by 2\n";
					}
					else if (startsWith(upgradeArgsStats[1], "3"))
					{
						std::cout << "Engine has been upgraded by 3\n";
					}
				}
			}
		}
	}

	playerProgress.close();

	/*playerProgress << "carID =  1";
	playerProgress << "Upgrade = Nitro :" << ", ";
	playerProgress.close();*/

}

void Manager::savePlayerProgress()
{

}

bool Manager::startsWith(std::string input, std::string keyWord)
{
	return input.substr(0, keyWord.length()) == keyWord;
}

FPSCamera* Manager::getCamera() {
	return camera;
}