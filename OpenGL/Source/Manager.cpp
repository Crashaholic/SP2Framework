#include "Manager.h"
#include "LoadTGA.h"
#include "Player.h"
#include "AICar.h"
#include "LevitationPad.h"
#include "GUIManager.h"

Manager* Manager::instance = nullptr;

Manager::Manager()
{
	shaders["lit"] = new ShaderProgram("Shader//Texture.vert", "Shader//Blending.frag");
	shaders["text"] = new ShaderProgram("Shader//Text.vert", "Shader//Text.frag");
	shaders["overlay"] = new ShaderProgram("Shader//UI.vert", "Shader//UI.frag");


	carOneUnlock = true;
	carTwoUnlock = false;
	carThreeUnlock = false;
	money = 0;

	currentLevel = "game";

	std::string path = "Data//Level";
	for (const auto & entry : std::experimental::filesystem::directory_iterator(path)) {
		currentLevel = Utility::splitLine(entry.path().filename().string(), '.')[0];
		std::cout << currentLevel << std::endl;
		levels[currentLevel] = new Level(entry.path().string().c_str());
	}

	mainmenu = new Camera(Vector3(1, 1, 1));
}


Manager::~Manager()
{

	delete mainmenu;

	savePlayerProgress();

	for (auto const& shader : shaders)
		if (shader.second != nullptr)
			delete shader.second;

	for (auto const& level : levels)
		if (level.second != nullptr)
			delete level.second;

}


Manager* Manager::getInstance()
{
	if (instance == nullptr)
		instance = new Manager();
	return instance;
}

void Manager::setLevel(std::string name) {
	currentLevel = name;

	if (name == "game") {
		Primitive* quad = Primitives::generateQuad(Color(1, 1, 1));
		levels[currentLevel]->spawnObject(new Mesh("skyboxFront", quad, LoadTGA("Image//front.tga")));
		levels[currentLevel]->spawnObject(new Mesh("skyboxTop", quad, LoadTGA("Image//top.tga")));
		levels[currentLevel]->spawnObject(new Mesh("skyboxBottom", quad, LoadTGA("Image//bottom.tga")));
		levels[currentLevel]->spawnObject(new Mesh("skyboxLeft", quad, LoadTGA("Image//left.tga")));
		levels[currentLevel]->spawnObject(new Mesh("skyboxRight", quad, LoadTGA("Image//right.tga")));
		levels[currentLevel]->spawnObject(new Mesh("skyboxBack", quad, LoadTGA("Image//back.tga")));

		Primitive* axes = Primitives::generateAxes();
		levels[currentLevel]->spawnObject(new Mesh("axes", axes, 0, false, false, "environment", Mesh::DRAW_LINES));
		levels[currentLevel]->spawnObject(new Mesh("playerAxes", axes, 0, false, false, "environment", Mesh::DRAW_LINES));

		dynamic_cast<Player*>(levels[currentLevel]->getObject("human"))->setCar(dynamic_cast<Car*>(levels[currentLevel]->getObject("car")));
	}
}


Level* Manager::getLevel()
{
	return levels[currentLevel];
}

ShaderProgram* Manager::getShader(std::string name)
{
	return shaders[name];
}

std::map<std::string, ShaderProgram*>* Manager::getShaders()
{
	return &shaders;
}

Camera* Manager::getCamera() {
	if (currentLevel == "game") {
		return dynamic_cast<Player*>(levels[currentLevel]->getObject("human"))->getCamera();
	}
	else if (currentLevel == "pregame") {
		return mainmenu;
	}
}

std::string& Manager::getLevelName() {
	return currentLevel;
}


void Manager::loadPlayerProgress()
{
	std::fstream playerProgress; //input
	std::string line;
	int carID = 0;

	playerProgress.open("playerProgress.txt", std::fstream::in);
	if (playerProgress.is_open())
	{
		while (getline(playerProgress, line))
		{
			std::vector<std::string> args = Utility::splitLine(line, '=');

			if (Utility::startsWith(line, "Money"))
			{
				/*std::cout << "ID has been secured";*/
				money = std::stoi(args[1]);
				std::cout << "Money: " << money << std::endl;
			}

			if (Utility::startsWith(line, "ID"))
			{
				/*std::cout << "ID has been secured";*/
				carID = std::stoi(args[1]);
				std::cout << "Car ID " << carID << std::endl;
			}

			if (Utility::startsWith(line, "Upgrade"))
			{
				std::cout << "Car has been upgraded \n";
				std::vector<std::string> upgradeArgs = Utility::splitLine(args[1], ',');
				if (Utility::startsWith(upgradeArgs[0], "Nitro"))
				{
					std::vector<std::string> upgradeArgsStats = Utility::splitLine(upgradeArgs[0], ':');
					if (Utility::startsWith(upgradeArgsStats[1], "1"))
					{
						std::cout << "Nitro has been upgraded by 1\n";
					}
					else if (Utility::startsWith(upgradeArgsStats[1], "2"))
					{
						std::cout << "Nitro has been upgraded by 2\n";
					}
					else if (Utility::startsWith(upgradeArgsStats[1], "3"))
					{
						std::cout << "Nitro has been upgraded by 3\n";
					}
				}
				if (Utility::startsWith(upgradeArgs[1], "Tire"))
				{
					std::vector<std::string> upgradeArgsStats = Utility::splitLine(upgradeArgs[1], ':');
					if (Utility::startsWith(upgradeArgsStats[1], "1"))
					{
						std::cout << "Tire has been upgraded by 1\n";
					}
					else if (Utility::startsWith(upgradeArgsStats[1], "2"))
					{
						std::cout << "Tire has been upgraded by 2\n";
					}
					else if (Utility::startsWith(upgradeArgsStats[1], "3"))
					{
						std::cout << "Tire has been upgraded by 3\n";
					}
				}
				if (Utility::startsWith(upgradeArgs[2], "Engine"))
				{
					std::vector<std::string> upgradeArgsStats = Utility::splitLine(upgradeArgs[2], ':');
					if (Utility::startsWith(upgradeArgsStats[1], "1"))
					{
						std::cout << "Engine has been upgraded by 1\n";
					}
					else if (Utility::startsWith(upgradeArgsStats[1], "2"))
					{
						std::cout << "Engine has been upgraded by 2\n";
					}
					else if (Utility::startsWith(upgradeArgsStats[1], "3"))
					{
						std::cout << "Engine has been upgraded by 3\n";
					}
				}
			}
		}
	}

	playerProgress.close();
}

void Manager::savePlayerProgress()
{
	std::fstream playerProgress; //input
	std::string line;
	std::string moneyString;

	moneyString = std::to_string(money);

	playerProgress.open("playerProgress.txt", std::fstream::out | std::fstream::trunc);

	if (playerProgress.is_open())
	{
		playerProgress << "Money=" << moneyString << "\n";
			if (carOneUnlock == true)
			{
				playerProgress << "ID=1\n";
				playerProgress << "Upgrade=" <<"\n";
			}
			if (carTwoUnlock == true)
			{
				playerProgress << "ID=2\n";
			}
			if (carThreeUnlock == true)
			{
				playerProgress << "ID=3\n";
			}
	}
	playerProgress.close();
}

