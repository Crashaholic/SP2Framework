#include "Manager.h"
#include "LoadTGA.h"
#include "Player.h"
#include "AICar.h"
#include "LevitationPad.h"

Manager* Manager::instance = nullptr;

Manager::Manager()
{
	shaders["lit"] = new ShaderProgram("Shader//Texture.vert", "Shader//Blending.frag");
	shaders["text"] = new ShaderProgram("Shader//Text.vert", "Shader//Text.frag");
	shaders["overlay"] = new ShaderProgram("Shader//UI.vert", "Shader//UI.frag");
	for(int i = 0; i < 2; i++)
		lightSources.push_back(new LightSource());

	carOneUnlock = true;
	carTwoUnlock = false;
	carThreeUnlock = false;
	money = 0;


	tree = new QuadTree(Vector3(0, 0, 0), Vector3(100, 0, 100));
	//loadPlayerProgress();
	loadMap();
}


Manager::~Manager()
{
	savePlayerProgress();
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

			if (startsWith(line, "Money"))
			{
				/*std::cout << "ID has been secured";*/
				money = std::stoi(args[1]);
				std::cout << "Money: " << money << std::endl;
			}

			if (startsWith(line, "ID"))
			{
				/*std::cout << "ID has been secured";*/
				carID = std::stoi(args[1]);
				std::cout << "Car ID " << carID << std::endl;
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
					std::vector<std::string> upgradeArgsStats = Utility::splitLine(upgradeArgs[1], ':');
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
					std::vector<std::string> upgradeArgsStats = Utility::splitLine(upgradeArgs[2], ':');
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

void Manager::loadMap() {
	std::ifstream handle("Data//level.txt");
	if (!handle.is_open()) {
		std::cout << "[Error] Could not load level.txt!" << std::endl;
	}

	std::string line;
	struct Object {
		std::map<std::string, std::string> values;
		void Set(std::string key, std::string value)
		{
			values[key] = value;
		}

		std::string Get(std::string key)
		{
			return values[key];
		}
	};

	std::vector<Object*> objs;
	Object* current = nullptr;

	while (std::getline(handle, line)) {
		std::vector<std::string> args = Utility::splitLine(line, '=');

		if (startsWith(line, " ") || line == "")
		{
			continue;
		}
		else if (startsWith(line, "name"))
		{
			if (current != nullptr) objs.push_back(current);
			current = new Object();
		}
		current->Set(args[0], args[1]);
	}

	if (current != nullptr) objs.push_back(current);

	for (int i = 0; i < (int)objs.size(); i++) {
		current = objs[i];
		bool collision = (current->Get("collision") == "true" ? true : false);
		bool gravity = (current->Get("gravity") == "true" ? true : false);
		unsigned int textureID = LoadTGA(current->Get("texture").c_str());
		Primitive* primitive = Primitives::loadModel(current->Get("model").c_str());

		std::string type = current->Get("type");
		Mesh* m = nullptr;
		if (type == "player") {
			m = new Player(current->Get("name").c_str(), primitive, textureID);
		}
		else if (type == "car") {
			m = new Car(current->Get("name").c_str(), primitive, textureID);
		}
		else if (type == "pad") {
			m = new LevitationPad(current->Get("name").c_str(), primitive, textureID, std::stof(current->Get("levitation")));
		}
		else if (type == "ai") {
			m = new AICar(current->Get("name").c_str(), primitive, textureID);
		}
		else {
			m = new Mesh(current->Get("name").c_str(), primitive, textureID, collision, gravity);
		}
		std::vector<std::string> pos = Utility::splitLine(current->Get("position"), ',');
		std::vector<std::string> rot = Utility::splitLine(current->Get("rotation"), ',');
		m->position = Vector3(std::stof(pos[0]), std::stof(pos[1]), std::stof(pos[2]));
		m->rotation = Vector3(std::stof(rot[0]), std::stof(rot[1]), std::stof(rot[2]));
		spawnObject(m);
	}

	handle.close();
}

bool Manager::startsWith(std::string input, std::string keyWord)
{
	return input.substr(0, keyWord.length()) == keyWord;
}

QuadTree* Manager::getTree()
{
	return tree;
}