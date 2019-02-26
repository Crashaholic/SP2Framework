#include "SceneA2.h"
#include "GL/glew.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include "Primitives.h"
#include "shader.hpp"
#include "ShaderProgram.h"
#include "Collision.h"
#include "QuadTree.h"
#include "AICar.h"
#include "LevitationPad.h"

SceneA2::SceneA2()
{
	state_MainMenu = true;
	state_InGame = true;
	state_Race = false;
}


SceneA2::~SceneA2()
{
}

void SceneA2::Init()
{

	manager = Manager::getInstance();

	Engine.init();
	Music[BGM_MAIN].load("Music//BGM_MainMenu.wav");
	Music[BGM_INGAME].load("Music//BGM_InGame.wav");


	/*manager->loadPlayerProgress();*/
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	elapsedTimeCounter = bounceTimeCounter = lastTimed = 0.0f;
	lastFramesPerSecond = framesPerSecond = 1;

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	InitShaderProperties();


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	loadFlag = false;

}

void SceneA2::loadProgress() {
	if (loadFlag == false && dynamic_cast<Player*>(manager->getLevel()->getObject("player")) != nullptr)
	{
		manager->loadPlayerProgress();
		loadFlag = true;
	}
}

void SceneA2::Render()
{
	gui = GUIManager::getInstance();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glBindVertexArray(m_vertexArrayID);
	GenerateText();
	Manager::getInstance()->getLevel()->Render();




	

}

void SceneA2::RenderScene()
{

}

void SceneA2::GenerateText() 
{

	GUIText* text = nullptr;
	Level* level = manager->getLevel();
	GUIText* fps = gui->renderText("bahnschrift", 0, 10, "FPS: " + std::to_string(lastFramesPerSecond), 0.4f, Color(0, 1, 0));
	Manager::getInstance()->getLevel()->getScreen()->addText(fps);



	if (manager->getLevelName() == "game")
	{
		if (level->getScreenName() == "gameplay")
		{


			Player* player = dynamic_cast<Player*>(Manager::getInstance()->getLevel()->getObject("player"));

			if (player->isInVehicle)
			{

				std::string velocity;
				Color color;
				dynamic_cast<Car*>(manager->getLevel()->getObject("car"))->getVelocity(velocity, color);
				text = gui->renderText("digital", 40, Application::winHeight / 2.0f - 20, velocity, 0.4f, color, TEXT_ALIGN_BOTTOM);
				level->getScreen()->addText(text);

				dynamic_cast<Car*>(manager->getLevel()->getObject("car2"))->getVelocity(velocity, color);
				text = gui->renderText("digital", 40, Application::winHeight / 2.0f + 20, velocity, 0.4f, color, TEXT_ALIGN_TOP);
				level->getScreen()->addText(text);

			}
			else
			{
				Mesh* shop = Manager::getInstance()->getLevel()->getObject("shopkeeper");

				if ((player->position - shop->position).Length() <= 3.0f)
				{
					text = gui->renderText("default", Application::winWidth / 2.0f, Application::winHeight / 2.0f, "Press E to open shop", 0.5f, Color(0, 1, 0), TEXT_ALIGN_MIDDLE);
					level->getScreen()->addText(text);
				}

				Mesh* race = Manager::getInstance()->getLevel()->getObject("racenpc");
				if ((player->position - race->position).Length() <= 3.0f)
				{
					text = gui->renderText("default", Application::winWidth / 2.0f, Application::winHeight / 2.0f, "Press E to enter race", 0.5f, Color(1, 0.5, 0), TEXT_ALIGN_MIDDLE);
					level->getScreen()->addText(text);
				}

			}

			//std::string money = manager->getMoney();
			//std::string carStats;
			Color color;

			//R: Player UI (Money)
			//dynamic_cast<Player*>(manager->getLevel()->getObject("player"))->getMoneyText(money, color);
			//GUIText* playerMoney = gui->renderText("digital", 40, Application::winHeight / 2.0f - 100, money, 0.4f, color, TEXT_ALIGN_TOP);
			//Manager::getInstance()->getLevel()->getScreen()->addText(playerMoney);

			////R: Player UI (Engine)
			//dynamic_cast<Car*>(manager->getLevel()->getObject("car"))->getEngineTierText(carStats, color);
			//GUIText* carTierStats = gui->renderText("digital", 20, Application::winHeight / 2.0f - 120, carStats, 0.2f, color, TEXT_ALIGN_LEFT);
			//Manager::getInstance()->getLevel()->getScreen()->addText(carTierStats);

			////R: Player UI (Nitro)
			//dynamic_cast<Car*>(manager->getLevel()->getObject("car"))->getNitroTierText(carStats, color);
			//carTierStats = gui->renderText("digital", 20, Application::winHeight / 2.0f - 140, carStats, 0.2f, color, TEXT_ALIGN_LEFT);
			//Manager::getInstance()->getLevel()->getScreen()->addText(carTierStats);

			////R: Player UI (Tires)
			//dynamic_cast<Car*>(manager->getLevel()->getObject("car"))->getTireTierText(carStats, color);
			//carTierStats = gui->renderText("digital", 20, Application::winHeight / 2.0f - 160, carStats, 0.2f, color, TEXT_ALIGN_LEFT);
			//Manager::getInstance()->getLevel()->getScreen()->addText(carTierStats);



		}else if (level->getScreenName() == "playermode") {
			text = gui->renderText("default", 500, 83, "Level Selection", 0.5f, Color(1, 1, 1), TEXT_ALIGN_MIDDLE);
			Manager::getInstance()->getLevel()->getScreen()->addText(text);

			text = gui->renderText("default", 265, 315, "Singleplayer", 0.35f, Color(1, 1, 1), TEXT_ALIGN_MIDDLE);
			Manager::getInstance()->getLevel()->getScreen()->addText(text);

			text = gui->renderText("default", 265, 470, "Multiplayer", 0.35f, Color(1, 1, 1), TEXT_ALIGN_MIDDLE);
			Manager::getInstance()->getLevel()->getScreen()->addText(text);


		}
		else if (level->getScreenName() == "shop") {
			text = gui->renderText("default", 170, 80, "buy car", 0.35f, Color(1, 1, 1), TEXT_ALIGN_MIDDLE);
			level->getScreen()->addText(text);

			text = gui->renderText("default", 160, 180, "upgrade nitro", 0.30f, Color(1, 1, 1), TEXT_ALIGN_MIDDLE);
			level->getScreen()->addText(text);

			text = gui->renderText("default", 160, 280, "upgrade tire", 0.35f, Color(1, 1, 1), TEXT_ALIGN_MIDDLE);
			level->getScreen()->addText(text);

			text = gui->renderText("default", 160, 380, "upgrade engine", 0.30f, Color(1, 1, 1), TEXT_ALIGN_MIDDLE);
			level->getScreen()->addText(text);

			text = gui->renderText("default", 180, 480, "undo", 0.35f, Color(1, 1, 1), TEXT_ALIGN_MIDDLE);
			level->getScreen()->addText(text);

			text = gui->renderText("default", 180, 580, "exit", 0.35f, Color(1, 1, 1), TEXT_ALIGN_MIDDLE);
			level->getScreen()->addText(text);
		}else if (manager->getLevel()->getScreenName() == "confirmation") {
			GUIText* text = gui->renderText("default", 512, 280, "Yes", 0.35f, Color(1, 1, 0), TEXT_ALIGN_MIDDLE);
			Manager::getInstance()->getLevel()->getScreen()->addText(text);

			text = gui->renderText("default", 512, 480, "No", 0.30f, Color(1, 1, 0), TEXT_ALIGN_MIDDLE);
			Manager::getInstance()->getLevel()->getScreen()->addText(text);
		}
		else if (manager->getLevel()->getScreenName() == "confirmationundo") {
			GUIText* text = gui->renderText("default", 512, 280, "Yes", 0.35f, Color(1, 1, 0), TEXT_ALIGN_MIDDLE);
			Manager::getInstance()->getLevel()->getScreen()->addText(text);

			text = gui->renderText("default", 512, 480, "No", 0.30f, Color(1, 1, 0), TEXT_ALIGN_MIDDLE);
			Manager::getInstance()->getLevel()->getScreen()->addText(text);
		}
		else if (manager->getLevel()->getScreenName() == "carselection") {
			GUIText* text = gui->renderText("default", 512, 80, "car Two", 0.35f, Color(1, 1, 1), TEXT_ALIGN_MIDDLE);
			Manager::getInstance()->getLevel()->getScreen()->addText(text);

			text = gui->renderText("default", 512, 180, "car Three", 0.30f, Color(1, 1, 1), TEXT_ALIGN_MIDDLE);
			Manager::getInstance()->getLevel()->getScreen()->addText(text);

			text = gui->renderText("default", 512, 280, "car Four", 0.35f, Color(1, 1, 1), TEXT_ALIGN_MIDDLE);
			Manager::getInstance()->getLevel()->getScreen()->addText(text);
		}

		else if (level->getScreenName() == "ingame")
		{
			RACE_STATE state = manager->getGameState();

			if (state == RACE_STARTING)
			{
				int secs = (int)manager->getRaceStartCountdown();
				std::string drawText;

				if (secs == 5)
				{
					drawText = "Match Starting";
				}
				else if (secs == 4)
				{
					drawText = "Get Ready!";
				}
				else if(secs >= 1 && secs < 4)
				{
					drawText = std::to_string(secs);
				}
				else
				{
					drawText = "Go!";
				}

				text = gui->renderText("digital", Application::winWidth / 2.0f - 10.0f,
					Application::winHeight / 2.0f, drawText, 0.5f, Color(1, 0, 0), TEXT_ALIGN_MIDDLE);
				level->getScreen()->addText(text);
			}
			else if (state == RACE_STARTED) {





				Car* car = dynamic_cast<Car*>(level->getObject("car"));

				if (car->hasFinished()) {

					IRender* grayTexture = level->getScreen()->getItem("endgamebot");
					if (!grayTexture->isEnabled()) grayTexture->setEnabled(true);


					text = gui->renderText("digital", Application::winWidth / 2.0f - 10.0f,
						100, std::to_string(car->getTiming()), 0.5f, Color(1, 0, 0), TEXT_ALIGN_MIDDLE);
					level->getScreen()->addText(text);
				}
				else {
					text = gui->renderText("digital", Application::winWidth / 2.0f - 10.0f,
						100, std::to_string(manager->getPlacement("car")), 0.5f, Color(1, 0, 0), TEXT_ALIGN_MIDDLE);
					level->getScreen()->addText(text);

				}

				Car* car2 = dynamic_cast<Car*>(level->getObject("car2"));
				if (car2->hasFinished()) {

					IRender* grayTexture = level->getScreen()->getItem("endgametop");
					if (!grayTexture->isEnabled()) grayTexture->setEnabled(true);

					text = gui->renderText("digital", Application::winWidth / 2.0f - 10.0f,
						Application::winHeight / 2.0f + 100, std::to_string(car2->getTiming()), 0.5f, Color(1, 0, 0), TEXT_ALIGN_MIDDLE);
					level->getScreen()->addText(text);
				}
				else {

					text = gui->renderText("digital", Application::winWidth / 2.0f - 10.0f,
						Application::winHeight / 2.0f + 100, std::to_string(manager->getPlacement("car2")), 0.5f, Color(1, 0, 0), TEXT_ALIGN_MIDDLE);
					level->getScreen()->addText(text);

				}

				if (car->hasFinished() && car2->hasFinished()) {
					level->setScreen("endgame");
					manager->setGameState(RACE_COMPLETED);
				}

			}

		}
		else if (level->getScreenName() == "endgame") {
			Car* car = dynamic_cast<Car*>(level->getObject("car"));
			Car* car2 = dynamic_cast<Car*>(level->getObject("car2"));
			std::string winner = (car->getTiming() < car2->getTiming()) ? "Player1 wins!" : "Player2 wins!";

			text = gui->renderText("digital", Application::winWidth / 2.0f - 20,
				100, winner, 0.5f, Color(1, 1, 1), TEXT_ALIGN_MIDDLE);
			level->getScreen()->addText(text);
			
			text = gui->renderText("digital", Application::winWidth / 2.0f - 70,
				Application::winHeight / 2.0f - 25, "Player One's Timing: " + std::to_string(car->getTiming()), 0.3f, Color(1, 1, 0), TEXT_ALIGN_MIDDLE);
			level->getScreen()->addText(text);

			text = gui->renderText("digital", Application::winWidth / 2.0f - 70,
				Application::winHeight / 2.0f + 25, "Player Two's Timing: " + std::to_string(car2->getTiming()), 0.3f, Color(1, 1, 0), TEXT_ALIGN_MIDDLE);
			level->getScreen()->addText(text);
			

			text = gui->renderText("default", 502, 565, "Next Level", 0.4f, Color(1, 1, 1), TEXT_ALIGN_MIDDLE);
			level->getScreen()->addText(text);

			text = gui->renderText("default", 356, 664, "Restart", 0.4f, Color(1, 1, 1), TEXT_ALIGN_MIDDLE);
			level->getScreen()->addText(text);

			text = gui->renderText("default", 656, 664, "Main Menu", 0.4f, Color(1, 1, 1), TEXT_ALIGN_MIDDLE);
			level->getScreen()->addText(text);

			
		}
	}
	else if (manager->getLevelName() == "pregame") {

		if (level->getScreenName() == "mainmenu") {

			text = gui->renderText("default", 196, 150, "Play", 0.4f, Color(1, 1, 1), TEXT_ALIGN_MIDDLE);
			Manager::getInstance()->getLevel()->getScreen()->addText(text);

			text = gui->renderText("default", 196, 315, "Tutorial", 0.35f, Color(1, 1, 1), TEXT_ALIGN_MIDDLE);
			Manager::getInstance()->getLevel()->getScreen()->addText(text);

			text = gui->renderText("default", 196, 480, "Options", 0.35f, Color(1, 1, 1), TEXT_ALIGN_MIDDLE);
			Manager::getInstance()->getLevel()->getScreen()->addText(text);

			text = gui->renderText("default", 196, 645, "Exit", 0.4f, Color(1, 1, 1), TEXT_ALIGN_MIDDLE);
			Manager::getInstance()->getLevel()->getScreen()->addText(text);
		}
		else if (level->getScreenName() == "gameselection")
		{
			text = gui->renderText("default", 196, 150, "New Game", 0.35f, Color(1, 1, 1), TEXT_ALIGN_MIDDLE);
			Manager::getInstance()->getLevel()->getScreen()->addText(text);

			text = gui->renderText("default", 196, 315, "Load Game", 0.35f, Color(1, 1, 1), TEXT_ALIGN_MIDDLE);
			Manager::getInstance()->getLevel()->getScreen()->addText(text);
		}
	}
}






void SceneA2::InitShaderProperties()
{
	ShaderProgram* lit = manager->getShader("lit");
	lit->use();
	std::vector<LightSource*>* lightSources = manager->getLevel()->getLightSources();
	for (int i = 0; i < (int)lightSources->size(); i++) {
		lightSources->at(i)->setProperties();
	}
	lit->setUniform("numLights", LightSource::lightCount);
	lit->updateUniforms();
}

void SceneA2::playMusic()
{
	if (state_MainMenu)
	{
		Engine.play(Music[BGM_MAIN]);
		state_MainMenu = false;
	}
	if (state_InGame)
	{
		Engine.play(Music[BGM_INGAME]);
		state_InGame = false;
	}
	if (!state_Race)
	{
		Engine.play(Music[BGM_RACE]);
	}

}


void SceneA2::Update(double dt)
{
	//playMusic();

	// Bounce Time
	if (bounceTimeCounter <= 0.0f) {

		static float LSPEED = 10.0f;

		// Enable Culling
		if (Application::IsKeyPressed('1'))
		{
			glEnable(GL_CULL_FACE);
		}
		// Disable Culling
		else if (Application::IsKeyPressed('2'))
		{
			glDisable(GL_CULL_FACE);
		}
		// Fill
		else if (Application::IsKeyPressed('3'))
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		// Wireframe
		else if (Application::IsKeyPressed('4'))
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
	
	}

	
	if (manager->getGameState() == RACE_STARTING)
		manager->updateStartCountdown(dt);
		

	// Update logic for all objects
	manager->getLevel()->Update(dt);

	// Bounce Time & Elapsed Time
	bounceTimeCounter -= (float) dt;
	elapsedTimeCounter += (float)dt;

	// FPS Counter
	framesPerSecond++;
	if (elapsedTimeCounter - lastTimed > 1.0f)
	{
		lastTimed = elapsedTimeCounter;
		lastFramesPerSecond = framesPerSecond;
		framesPerSecond = 1;
	}

}

void SceneA2::Exit()
{
	
	Engine.deinit();
	delete manager;
	delete gui;

	glDeleteVertexArrays(1, &m_vertexArrayID);
}


