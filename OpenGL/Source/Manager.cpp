#include "Manager.h"
#include "LoadTGA.h"
#include "Player.h"
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



	std::string path = "Data//Level";
	for (const auto & entry : std::experimental::filesystem::directory_iterator(path)) {
		currentLevel = Utility::splitLine(entry.path().filename().string(), '.')[0];
		std::cout << currentLevel << std::endl;
		levels[currentLevel] = new Level(entry.path().string().c_str(), &waypoints);
	}


	currentLevel = "pregame";
	setLevel("pregame");
	mainmenu = new Camera(Vector3(1, 8, 1));
	levels[currentLevel]->setScreen(levels[currentLevel]->getScreenName());
	raceStartCountdown = -1.0f;
	gameState = RACE_IDLE;
	shop = new Shop();
	loadCars();
	srand((unsigned)time(0));

}


Manager::~Manager()
{


	delete shop;
	delete mainmenu;



	for (int i = 0; i < waypoints.size(); i++)
		if(waypoints[i] != nullptr)
			delete waypoints[i];

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

	if (name == "game" || name == "pregame") {

		loadPlayerProgress();
		Car* car = dynamic_cast<Car*>(levels[name]->getObject("car"));
		if (car != nullptr) {
			//delete levels[name];
			//levels[name] = new Level("Data//Level//game.txt", &waypoints);
		}

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

		
	}

	
}

void Manager::setGameState(RACE_STATE state)
{
	gameState = state;
	if (state == RACE_STARTING) {
		levels[currentLevel]->getScreen()->getItem("endgametop")->setEnabled(false);
		levels[currentLevel]->getScreen()->getItem("endgamebot")->setEnabled(false);
		raceStartCountdown = 6.0f;
	}
}

RACE_STATE Manager::getGameState()
{
	return gameState;
}

double Manager::getRaceStartCountdown()
{
	return raceStartCountdown;
}

Shop* Manager::getShop() {
	return shop;
}

void Manager::updateStartCountdown(double dt)
{
	raceStartCountdown -= dt;
	if (raceStartCountdown <= 0.0f)
	{
		gameState = RACE_STARTED;
		levels[currentLevel]->getScreen()->removeItem("starttextbackground");
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
		return dynamic_cast<Player*>(levels[currentLevel]->getObject("player"))->getCamera();
	}
	else if (currentLevel == "pregame") {
		return mainmenu;
	}
}

std::string& Manager::getLevelName() {
	return currentLevel;
}

std::vector<Waypoint*>* Manager::getWaypoints() {
	return &waypoints;
}

void Manager::loadCars() {
	std::ifstream handle("Data\\cars.txt");
	if (!handle.is_open()) {
		std::cout << "[Error] Could not load cars.txt!" << std::endl;
	}

	std::string line;

	std::vector<Object*> collection;
	std::string category = "";
	Object* current = nullptr;
	while (std::getline(handle, line))
	{
		std::vector<std::string> args = Utility::splitLine(line, '=');

		if (Utility::startsWith(line, " ") || line == "")
		{
			continue;
		}
		else if (Utility::startsWith(line, "car"))
		{
			current = new Object();
			collection.push_back(current);
		}
		current->Set(args[0], args[1]);

	}

	for (int i = 0; i < collection.size(); i++) {
		Object* obj = collection[i];
		shop->addItem(obj->Get("car"), std::stoi(obj->Get("price")));
		std::cout << obj->Get("car") << ": " << obj->Get("price") << std::endl;
	}


	handle.close();
}

void Manager::loadPlayerProgress()
{
	Player* player = dynamic_cast<Player*>(levels[currentLevel]->getObject("player"));
	if (player == nullptr) return;

	std::ifstream handle("Data\\playerprogress.txt");
	if (!handle.is_open()) {
		std::cout << "[Error] Could not load playerprogress.txt!" << std::endl;
	}

	std::string line;
	std::vector<Object*> collection;
	std::string category = "";
	Object* current = nullptr;
	while (std::getline(handle, line))
	{
		std::vector<std::string> args = Utility::splitLine(line, '=');

		if (Utility::startsWith(line, " ") || line == "")
		{
			continue;
		}
		else if (Utility::startsWith(line, "money")) {
			player->setMoney(std::stoi(args[1]));
			continue;
		}
		else if (Utility::startsWith(line, "car"))
		{
			current = new Object();
			collection.push_back(current);
		}
		current->Set(args[0], args[1]);

	}

	for (int i = 0; i < collection.size(); i++) {
		Object* obj = collection[i];
		std::vector<CarUpgrade*>* upgrades = player->getUpgrades();
		upgrades->push_back(new CarUpgrade(obj->Get("car"), std::stoi(obj->Get("nitro")), std::stoi(obj->Get("engine")), std::stoi(obj->Get("tyre"))));
	}
	
	std::vector<CarUpgrade*>* upgrades = player->getUpgrades();
	for (int i = 0; i < upgrades->size(); i++) {
		std::cout << upgrades->at(i)->getName() << ": N" << upgrades->at(i)->getTier("nitro")
			<< ", E" << upgrades->at(i)->getTier("engine") << ", T" << upgrades->at(i)->getTier("tyre") << std::endl;
	}
	
	

	handle.close();
}

void Manager::savePlayerProgress()
{

	Player* player = dynamic_cast<Player*>(levels[currentLevel]->getObject("player"));
	if (player == nullptr) return;

	std::ofstream handle("Data\\playerprogress.txt");
	std::string line;

	if (!handle.is_open()){
		std::cout << "[ERROR] Can't save to Data\\playerprogress.txt" << std::endl;
		return;
	}

	handle << "money=" << player->getMoney() << std::endl;

	std::vector<CarUpgrade*>* upgrades = player->getUpgrades();
	for (int i = 0; i < upgrades->size(); i++) {
		handle << "car=" << upgrades->at(i)->getName() << std::endl;
		handle << "nitro=" << upgrades->at(i)->getTier("nitro") << std::endl;
		handle << "engine=" << upgrades->at(i)->getTier("engine") << std::endl;
		handle << "tyre=" << upgrades->at(i)->getTier("tyre") << std::endl;
	}
	handle.close();
}


int Manager::getPlacement(std::string name) {
	
	Car* car1 = dynamic_cast<Car*>(levels[currentLevel]->getObject("car"));
	Car* car2 = dynamic_cast<Car*>(levels[currentLevel]->getObject("car2"));

	int lapDiff = car2->getLaps() - car1->getLaps();

	if (lapDiff > 0) {
		if (name == "car") {
			return 2;
		}
		else {
			return 1;
		}
	}
	else if (lapDiff < 0) {
		if (name == "car"){
			return 1;
		}
		else {
			return 2;
		}
	}
	else {

		//int car2ID = -1;
		//if (car2->getWaypointID() == waypoints.size() - 1) {
		//	if (car2->getLaps() != 1)
		//		car2ID = 11;
		//}
		//else {
		//	car2ID = car2->getWaypointID();
		//}

		//int carID = 0;
		//if (car1->getWaypointID() == 0) {
		//	if (car1->getLaps() != 1)
		//		carID = 11;
		//}
		//else {
		//	carID = car1->getWaypointID();
		//}


		int checkpointDiff = car2->getWaypointID() - car1->getWaypointID();
		if (checkpointDiff > 0) {
			return (name == "car") ? 2 : 1;
		}
		else if (checkpointDiff < 0) {
			return (name == "car") ? 1 : 2;
		}
		else {
			int nextWaypoint = (car1->getWaypointID() + 1 == waypoints.size() ? 0 : car1->getWaypointID() + 1);
			Vector3 carDiff = waypoints[nextWaypoint]->getOBB()->getPos() - car1->position;
			carDiff.y = 0;
			Vector3 car2Diff = waypoints[nextWaypoint]->getOBB()->getPos() - car2->position;
			car2Diff.y = 0;
			if (carDiff.Length() < car2Diff.Length()) {
				return (name == "car") ? 1 : 2;
			}
			else {
				return (name == "car2") ? 1 : 2;
			}
		}
	}
}