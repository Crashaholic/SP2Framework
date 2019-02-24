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



		}else if (level->getScreenName() == "playermode") {
			text = gui->renderText("default", 500, 83, "Level Selection", 0.5f, Color(1, 1, 1), TEXT_ALIGN_MIDDLE);
			Manager::getInstance()->getLevel()->getScreen()->addText(text);

			text = gui->renderText("default", 265, 315, "Singleplayer", 0.35f, Color(1, 1, 1), TEXT_ALIGN_MIDDLE);
			Manager::getInstance()->getLevel()->getScreen()->addText(text);

			text = gui->renderText("default", 265, 470, "Multiplayer", 0.35f, Color(1, 1, 1), TEXT_ALIGN_MIDDLE);
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
	lit = manager->getShader("lit");
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
	manager->savePlayerProgress();
	Engine.deinit();
	delete manager;
	delete gui;

	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(lit->getID());
}


