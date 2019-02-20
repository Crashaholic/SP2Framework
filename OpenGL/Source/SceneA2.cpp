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
	CreateMesh();

	Mtx44 projection;
	projection.SetToPerspective(45.0f, (float)Application::winWidth / (float)Application::winHeight, 0.1f, 10000.0f);
	projectionStack.LoadMatrix(projection);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void SceneA2::CreateMesh()
{
	// Skybox Planes
	Primitive* quad = Primitives::generateQuad(Color(1, 1, 1));
	manager->spawnObject(new Mesh("skyboxFront", quad, LoadTGA("Image//front.tga")));
	manager->spawnObject(new Mesh("skyboxTop", quad, LoadTGA("Image//top.tga")));
	manager->spawnObject(new Mesh("skyboxBottom", quad, LoadTGA("Image//bottom.tga")));
	manager->spawnObject(new Mesh("skyboxLeft", quad, LoadTGA("Image//left.tga")));
	manager->spawnObject(new Mesh("skyboxRight", quad, LoadTGA("Image//right.tga")));
	manager->spawnObject(new Mesh("skyboxBack", quad, LoadTGA("Image//back.tga")));

	Primitive* axes = Primitives::generateAxes();
	manager->spawnObject(new Mesh("axes", axes, 0, false, false, "environment", Mesh::DRAW_LINES));
	manager->spawnObject(new Mesh("playerAxes", axes, 0, false, false, "environment", Mesh::DRAW_LINES));

	player = dynamic_cast<Player*>(manager->getObject("human"));
	car = dynamic_cast<Car*>(manager->getObject("car"));
	player->setCar(car);

}

void SceneA2::Render()
{
	gui = GUIManager::getInstance();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//modelStack.LoadIdentity();

	//glBindFramebuffer(GL_FRAMEBUFFER, gui->FBO);
	//viewStack.LoadMatrix(player->getTopdownCamera()->LookAt());

	std::vector<LightSource*>* lightSources = manager->getLightSources();
	//for (int i = 0; i < (int)lightSources->size(); i++)
	//{
	//	lightSources->at(i)->updateAttributes(viewStack);
	//}

	//RenderScene();
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	modelStack.LoadIdentity();

	viewStack.LoadMatrix(player->getCamera()->LookAt());

	for (int i = 0; i < (int)lightSources->size(); i++)
	{
		lightSources->at(i)->updateAttributes(viewStack);
	}

	gui = GUIManager::getInstance();
	RenderScene();
	glDisable(GL_DEPTH_TEST);
	RenderUI();
	glEnable(GL_DEPTH_TEST);
}

void SceneA2::RenderScene()
{
	glBindVertexArray(m_vertexArrayID);

	RenderSkybox();
	lit->use();

	modelStack.PushMatrix();
	manager->getObject("playerAxes")->Translate(modelStack, car->position.x, car->position.y + car->getOBB()->getHalf().y, car->position.z);
	manager->getObject("playerAxes")->Rotate(modelStack, car->rotation.x, 1, 0, 0);
	manager->getObject("playerAxes")->Rotate(modelStack, car->rotation.y, 0, 1, 0);
	manager->getObject("playerAxes")->Rotate(modelStack, car->rotation.z, 0, 0, 1);
	RenderMesh(manager->getObject("playerAxes"), false);
	modelStack.PopMatrix();

	std::map<std::string, Mesh*>* objects = manager->getObjects();


	for (auto& obj : *objects)
	{

		Mesh* m = obj.second;

		std::string key = obj.first;
		if (Utility::startsWith(key, "skybox") || key == "axes" || key == "playerAxes") continue;

		if (obj.first == "ai" || obj.first == "car") {
			glDisable(GL_CULL_FACE);
		}

		modelStack.PushMatrix();
		m->ResetOBB();
		m->Translate(modelStack, m->position.x, m->position.y, m->position.z);
		m->Rotate(modelStack, m->rotation.x, 1, 0, 0);
		m->Rotate(modelStack, m->rotation.y, 0, 1, 0);
		m->Rotate(modelStack, m->rotation.z, 0, 0, 1);
		RenderMesh(m, true);
		modelStack.PopMatrix();

		if (obj.first == "ai" || obj.first == "car") {
			glEnable(GL_CULL_FACE);
		}
	}

}

void SceneA2::RenderUI() 
{
	gui->update(Application::mouse_x, Application::mouse_y, Application::winWidth, Application::winHeight);
	gui->renderUI();
	gui->renderText("bahnschrift", 0, 10, "FPS: " + std::to_string(lastFramesPerSecond), 0.4f, Color(0, 1, 0));

	//gui->renderText("default", 0, 300, "Waypoints: " + std::to_string(dynamic_cast<AICar*>(manager->getObject("ai"))->currentID), 0.4f, Color(0, 1, 0));
	if (!player->isInVehicle && (player->getCar()->position - player->position).Length() <= 6.0f)
		gui->renderText("default", 400, 300, "Press F to enter car", 0.4f, Color(0, 1, 0), TEXT_ALIGN_MIDDLE);

	
	Mesh* ai = manager->getObject("ai");

	gui->renderText("default", 0, 550, "AI Pos: " + std::to_string(ai->position.x) + "," + std::to_string(ai->position.y) + ","
		+ std::to_string(ai->position.z), 0.25f, Color(0, 1, 1));

	gui->renderText("default", 0, 500, "AI Vel: " + std::to_string(ai->velocity.x) + "," + std::to_string(ai->velocity.y) + ","
		+ std::to_string(ai->velocity.z), 0.25f, Color(1, 0, 0));

}

void SceneA2::RenderMesh(Mesh* mesh, bool enableLight, unsigned int shader)
{
	Mtx44 MVP, modelView, modelView_inverse_tranpose;
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	modelView = viewStack.Top() * modelStack.Top();

	lit->use();

	if (enableLight)
	{
		modelView_inverse_tranpose = modelView.GetInverse().GetTranspose();
		lit->setUniform("lightEnabled", 1);
		lit->setUniform("MV_inverse_transpose", modelView_inverse_tranpose.a);
		lit->setUniform("material.kAmbient", mesh->material.kAmbient.r, mesh->material.kAmbient.g, mesh->material.kAmbient.b);
		lit->setUniform("material.kDiffuse", mesh->material.kDiffuse.r, mesh->material.kDiffuse.g, mesh->material.kDiffuse.b);
		lit->setUniform("material.kSpecular", mesh->material.kSpecular.r, mesh->material.kSpecular.g, mesh->material.kSpecular.b);
		lit->setUniform("material.kShininess", mesh->material.kShininess);

	}
	else
	{
		lit->setUniform("lightEnabled", 0);
	}

	// Do normal one texture per mesh bind
	if (mesh->textureID > 0)
	{
		lit->setUniform("colorTextureEnabled", 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		lit->setUniform("colorTexture", 0);
	}
	else
	{
		lit->setUniform("colorTextureEnabled", 0);
	}

	lit->updateUniforms();

	mesh->Render(modelStack, viewStack, projectionStack, lit);

	if (mesh->textureID > 0)
		glBindTexture(GL_TEXTURE_2D, 0);
	
}



void SceneA2::RenderSkybox()
{
	lit->use();
	float length = 1000.0f;
	modelStack.PushMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0.0f, length / 2, 0.0f);
	modelStack.Rotate(-90.0f, 0, 1, 0);
	modelStack.Rotate(90.0f, 1, 0, 0);
	modelStack.Scale(length, length, 1.0f);
	RenderMesh(manager->getObject("skyboxTop"), false, lit->getID());
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0.0f, -length / 2, 0.0f);
	modelStack.Rotate(-90.0f, 1, 0, 0);
	modelStack.Scale(length, length, 1.0f);
	RenderMesh(manager->getObject("skyboxBottom"), false, lit->getID());
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0.0f, 0.0f, length / 2);
	modelStack.Rotate(180.0f, 1, 1, 0);
	modelStack.Rotate(-90.0f, 0, 0, 1);
	modelStack.Scale(length, length, 1.0f);
	RenderMesh(manager->getObject("skyboxFront"), false, lit->getID());
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0.0f, 0.0f, -length / 2);
	modelStack.Scale(length, length, 1.0f);
	RenderMesh(manager->getObject("skyboxBack"), false, lit->getID());
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-length / 2, 0.0f, 0.0f);
	modelStack.Rotate(90.0f, 0, 1, 0);
	modelStack.Scale(length, length, 1.0f);
	RenderMesh(manager->getObject("skyboxLeft"), false, lit->getID());
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(length / 2, 0.0f, 0.0f);
	modelStack.Rotate(-90.0f, 0, 1, 0);
	modelStack.Scale(length, length, 1.0f);
	RenderMesh(manager->getObject("skyboxRight"), false, lit->getID());
	modelStack.PopMatrix();

	modelStack.PopMatrix();
}

void SceneA2::InitShaderProperties()
{
	lit = manager->getShader("lit");
	lit->use();
	std::vector<LightSource*>* lightSources = manager->getLightSources();
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
		// Switch Camera Mode
		else if (Application::IsKeyPressed(VK_F5)) {
			player->switchCameraMode();
			bounceTimeCounter = 0.3f;
		}
		// Board Car
		else if (Application::IsKeyPressed('F')) {
			if (!player->isInVehicle && (player->getCar()->position - player->position).Length() <= 6.0f) {
				player->isInVehicle = true;
				player->getCar()->setOccupied(true);
				player->collisionEnabled = false;
				player->setCameraMode(FIXED_CAR);

			}
			else if (player->isInVehicle) {
				player->isInVehicle = false;
				player->getCar()->setOccupied(false);
				player->collisionEnabled = true;
				player->setCameraMode(FIRST_PERSON);
			}
			bounceTimeCounter = 0.3f;
		}
	
	}

	// Update logic for all objects
	std::map<std::string, Mesh*>* objects = manager->getObjects();
	for (auto const& object : *objects) {
		object.second->Update(dt);
	}

	Mtx44 projection;
	projection.SetToPerspective(45.0f, (float)Application::winWidth / (float)Application::winHeight, 0.1f, 10000.0f);
	projectionStack.LoadMatrix(projection);

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



/*

 Environment
manager->spawnObject(new Mesh("track", Primitives::loadModel("Models//track.obj"), LoadTGA("Models//human.tga"), false));
manager->spawnObject(new Mesh("ground", Primitives::loadModel("Models//ground.obj"), LoadTGA("Image//rock.tga"), true));

manager->spawnObject(new LevitationPad("pad1", Primitives::loadModel("Models//levitationpad.obj"), LoadTGA("Models//building.tga"), 5.0f));
manager->spawnObject(new Player("human", Primitives::loadModel("Models//player.obj"), LoadTGA("Models//human.tga")));
//manager->spawnObject(new Mesh("cube1", Primitives::loadModel("Models//cube1.obj"), LoadTGA("Image//rock.tga"), true));
//manager->spawnObject(new Mesh("cube2", Primitives::loadModel("Models//cube2.obj"), LoadTGA("Image//rock.tga"), true));
player = dynamic_cast<Player*>(manager->getObject("human"));
//manager->getObject("human")->loadChildren({ "human_body",
//	"human_leftthigh", "human_leftcalve", "human_leftshoe", "human_lefthand",
//	"human_rightthigh", "human_rightcalve", "human_rightshoe", "human_righthand" });

manager->spawnObject(new Car("car", Primitives::loadModel("Models//car.obj"), LoadTGA("Models//car.tga")));
car = dynamic_cast<Car*>(manager->getObject("car"));
car->loadChildren({ "car_steeringwheel" , "car_leftfrontwheel", "car_rightfrontwheel", "car_leftrearwheel", "car_rightrearwheel" });

player->setCar(car);
//manager->spawnObject(new Mesh("car", Primitives::loadModel("Models//car.obj"), LoadTGA("Models//car.tga")));

manager->spawnObject(new AICar("ai", Primitives::loadModel("Models//car.obj"), LoadTGA("Models//bridge.tga")));

*/