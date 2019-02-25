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


	//carOneUnlock = true;
	//carTwoUnlock = false;
	//carThreeUnlock = false;
	//money = 0;



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
}


Manager::~Manager()
{

	delete mainmenu;
	delete shop;

	savePlayerProgress(dynamic_cast<Player*>(getLevel()->getObject("player")));

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

void Manager::loadPlayerProgress(Player *player)
{
	std::fstream playerProgress; //input
	std::string line;
	int money = 0;
	int carID = 0;

	playerProgress.open("Data\\playerProgress.txt", std::fstream::in);
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
				player->setMoney(money);
			}

			if (Utility::startsWith(line, "ID"))
			{
				carID = std::stoi(args[1]);
				std::cout << "Car ID " << carID << std::endl;
				player->unlockCar(carID);
			}

			if (Utility::startsWith(line, "Upgrade"))
			{
				std::cout << "Car has been upgraded \n";
				std::vector<std::string> upgradeArgs = Utility::splitLine(args[1], ',');
				if (Utility::startsWith(upgradeArgs[0], "Nitro"))
				{
					std::vector<std::string> upgradeArgsStats = Utility::splitLine(upgradeArgs[0], ':');
					player->getCar()->setNitroTier(std::stoi(upgradeArgsStats[1]));
				}
				if (Utility::startsWith(upgradeArgs[1], "Tire"))
				{
					std::vector<std::string> upgradeArgsStats = Utility::splitLine(upgradeArgs[1], ':');
					player->getCar()->setTireTier(std::stoi(upgradeArgsStats[1]));
				}
				if (Utility::startsWith(upgradeArgs[2], "Engine"))
				{
					std::vector<std::string> upgradeArgsStats = Utility::splitLine(upgradeArgs[2], ':');
					player->getCar()->setEngineTier(std::stoi(upgradeArgsStats[1]));
				}
			}
		}
	}
	playerProgress.close();
}

void Manager::savePlayerProgress(Player *player)
{
	std::fstream playerProgress; //input
	std::string line;
	std::string moneyString;
	std::string nitroString;
	std::string tireString;
	std::string engineString;
	int money = player->getMoney();

	moneyString = std::to_string(money);
	nitroString = std::to_string(player->getCar()->getNitroTier());
	tireString = std::to_string(player->getCar()->getTireTier());
	engineString = std::to_string(player->getCar()->getEngineTier());

	playerProgress.open("Data\\playerProgress.txt", std::fstream::out | std::fstream::trunc);

	if (playerProgress.is_open())
	{
		playerProgress << "Money=" << moneyString << "\n";
			if (player->getCarsUnlocked(1) == true)
			{
				playerProgress << "ID=1\n";
			}
			if (player->getCarsUnlocked(2) == true)
			{
				playerProgress << "ID=2\n";
			}
			if (player->getCarsUnlocked(3) == true)
			{
				playerProgress << "ID=3\n";
			}
			if (player->getCarsUnlocked(4) == true)
			{
				playerProgress << "ID=4\n";
			}
			playerProgress << "Upgrade=Nitro:" << nitroString << ",Tire:" << tireString << ",Engine:" << engineString << "\n";
	}
	playerProgress.close();
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

		int car2ID = 0;
		if (car2->getWaypointID() == 0) {
			if (car2->getLaps() != 1)
				car2ID = 11;
		}
		else {
			car2ID = car2->getWaypointID();
		}

		int carID = 0;
		if (car1->getWaypointID() == 0) {
			if (car1->getLaps() != 1)
				carID = 11;
		}
		else {
			carID = car1->getWaypointID();
		}


		int checkpointDiff = car2ID - carID;
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