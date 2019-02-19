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
	manager->spawnObject(new Mesh("axes", axes, 0, false, Mesh::DRAW_LINES));
	manager->spawnObject(new Mesh("playerAxes", axes, 0, false, Mesh::DRAW_LINES));

	// Environment
	manager->spawnObject(new Mesh("track", Primitives::loadModel("Models//track.obj"), LoadTGA("Models//human.tga"), false));
	manager->spawnObject(new Mesh("ground", Primitives::loadModel("Models//ground.obj"), LoadTGA("Image//rock.tga"), true));
	manager->spawnObject(new Player("human", Primitives::loadModel("Models//human.obj"), LoadTGA("Models//human.tga")));
	//manager->spawnObject(new Mesh("cube1", Primitives::loadModel("Models//cube1.obj"), LoadTGA("Image//rock.tga"), true));
	//manager->spawnObject(new Mesh("cube2", Primitives::loadModel("Models//cube2.obj"), LoadTGA("Image//rock.tga"), true));
	player = dynamic_cast<Player*>(manager->getObject("human"));
	manager->getObject("human")->loadChildren({ "human_body",
		"human_leftthigh", "human_leftcalve", "human_leftshoe", "human_lefthand",
		"human_rightthigh", "human_rightcalve", "human_rightshoe", "human_righthand" });

	manager->spawnObject(new Car("car", Primitives::loadModel("Models//car.obj"), LoadTGA("Models//car.tga")));
	car = dynamic_cast<Car*>(manager->getObject("car"));
	car->loadChildren({ "car_steeringwheel" , "car_leftfrontwheel", "car_rightfrontwheel", "car_leftrearwheel", "car_rightrearwheel" });

	player->setCar(car);
	//manager->spawnObject(new Mesh("car", Primitives::loadModel("Models//car.obj"), LoadTGA("Models//car.tga")));

	manager->spawnObject(new AICar("ai", Primitives::loadModel("Models//car.obj"), LoadTGA("Models//bridge.tga")));
	

	//tree.Insert(player);
	//tree.Insert(car);
	//tree.Insert(manager->getObject("skyboxFront"));
	//tree.Insert(manager->getObject("skyboxTop"));
	//tree.Insert(manager->getObject("ground"));

	//std::vector<Mesh*> meshes = tree.queryMesh(Vector3(0,0,0), 20, 20);

}

void SceneA2::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	modelStack.LoadIdentity();

	viewStack.LoadMatrix(player->getCamera()->LookAt());

	std::vector<LightSource*>* lightSources = manager->getLightSources();
	for (int i = 0; i < (int)lightSources->size(); i++)
	{
		lightSources->at(i)->updateAttributes(viewStack);
	}

	gui = GUIManager::getInstance();
	RenderScene();
	RenderUI();
}

void SceneA2::RenderScene()
{
	glBindVertexArray(m_vertexArrayID);

	RenderSkybox();

	lit->use();

	// Environment
	modelStack.PushMatrix();
	manager->getObject("ground")->Rotate(modelStack, 180, 0, 1, 0);
	RenderMesh(manager->getObject("ground"), true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	manager->getObject("track")->Translate(modelStack, 0, 1.1f, 0.0);
	manager->getObject("track")->Rotate(modelStack, 180, 0, 1, 0);
	RenderMesh(manager->getObject("track"), true);
	modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//RenderMesh(manager->getObject("axes"), false);
	//modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//manager->getObject("playerAxes")->Translate(modelStack, player->position.x, player->position.y, player->position.z);
	//manager->getObject("playerAxes")->Rotate(modelStack, car->rotation.y - car->currentSteer, 0, 1, 0);
	//RenderMesh(manager->getObject("playerAxes"), false);
	//modelStack.PopMatrix();

	modelStack.PushMatrix();
	manager->getObject("human")->ResetOBB();
	manager->getObject("human")->Translate(modelStack, player->position.x, player->position.y, player->position.z);
	manager->getObject("human")->Rotate(modelStack, player->rotation.x, 1, 0, 0);
	manager->getObject("human")->Rotate(modelStack, player->rotation.y, 0, 1, 0);
	manager->getObject("human")->Rotate(modelStack, player->rotation.z, 0, 0, 1);
	RenderMesh(manager->getObject("human"), true);



	std::vector<Mesh*>* bodyChildren = manager->getObject("human")->getChildren();
	for (int i = 0; i < (int)bodyChildren->size(); i++)
	{
		modelStack.PushMatrix();
		RenderMesh(bodyChildren->at(i), true);
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//RenderMesh(manager->getObject("cube1"), true);
	//modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//RenderMesh(manager->getObject("cube2"), true);
	//modelStack.PopMatrix();

	glDisable(GL_CULL_FACE); // Disable Cull Face for Car
	modelStack.PushMatrix();
	manager->getObject("car")->ResetOBB();
	manager->getObject("car")->Translate(modelStack, car->position.x, car->position.y, car->position.z);
	manager->getObject("car")->Rotate(modelStack, car->rotation.x, 1, 0, 0);
	manager->getObject("car")->Rotate(modelStack, car->rotation.y, 0, 1, 0);
	manager->getObject("car")->Rotate(modelStack, -car->currentSteer, 0, 1, 0);
	manager->getObject("car")->Rotate(modelStack, car->rotation.z, 0, 0, 1);
	RenderMesh(manager->getObject("car"), true);
	std::vector<Mesh*>* carBody = car->getChildren();
	for (int i = 0; i < (int)carBody->size(); i++)
	{
		modelStack.PushMatrix();
		//if (i == 0)
		//{
		//	Mesh* steeringWheel = carBody->at(0);
		//	steeringWheel->Translate(modelStack, 0, 1.75f, 0.7f);
		//	steeringWheel->Rotate(modelStack, car->steeringWheelAngle * 2.4f, 0, 0, 1);

		//}
		//else if (i == 1)
		//{
		//	carBody->at(i)->Translate(modelStack, 1.55f, 0.72f, 2.2f);
		//	carBody->at(i)->Rotate(modelStack, -car->steeringWheelAngle, 0, 1, 0);
		//	carBody->at(i)->Rotate(modelStack, car->wheelAngle, 1, 0, 0);

		//}
		//else if (i == 2)
		//{
		//	carBody->at(i)->Translate(modelStack, -1.55f, 0.72f, 2.2f);
		//	carBody->at(i)->Rotate(modelStack, -car->steeringWheelAngle, 0, 1, 0);
		//	carBody->at(i)->Rotate(modelStack, car->wheelAngle, 1, 0, 0);
		//}
		//else if (i == 3)
		//{
		//	carBody->at(i)->Translate(modelStack, 1.55f, 0.73f, -2.85f);
		//	carBody->at(i)->Rotate(modelStack, car->wheelAngle, 1, 0, 0);
		//}
		//else if (i == 4)
		//{
		//	carBody->at(i)->Translate(modelStack, -1.55f, 0.73f, -2.85f);
		//	carBody->at(i)->Rotate(modelStack, car->wheel Angle, 1, 0, 0);
		//}
		RenderMesh(carBody->at(i), true);
		modelStack.PopMatrix();
	}


	modelStack.PopMatrix();
	glEnable(GL_CULL_FACE);

	glDisable(GL_CULL_FACE); // Disable Cull Face for Car
	modelStack.PushMatrix();
	AICar* ai = dynamic_cast<AICar*>(manager->getObject("ai"));
	ai->ResetOBB();

	ai->Translate(modelStack, ai->position.x, ai->position.y, ai->position.z);
	ai->Rotate(modelStack, ai->rotation.x, 1, 0, 0);
	ai->Rotate(modelStack, ai->rotation.y, 0, 1, 0);
	ai->Rotate(modelStack, -ai->currentSteer, 0, 1, 0);
	ai->Rotate(modelStack, ai->rotation.z, 0, 0, 1);
	RenderMesh(ai, true);
	//std::vector<Mesh*>* aiBody = ai->getChildren();
	//for (int i = 0; i < (int)aiBody->size(); i++)
	//{
	//	modelStack.PushMatrix();
	//	//if (i == 0)
	//	//{
	//	//	Mesh* steeringWheel = carBody->at(0);
	//	//	steeringWheel->Translate(modelStack, 0, 1.75f, 0.7f);
	//	//	steeringWheel->Rotate(modelStack, car->steeringWheelAngle * 2.4f, 0, 0, 1);

	//	//}
	//	//else if (i == 1)
	//	//{
	//	//	carBody->at(i)->Translate(modelStack, 1.55f, 0.72f, 2.2f);
	//	//	carBody->at(i)->Rotate(modelStack, -car->steeringWheelAngle, 0, 1, 0);
	//	//	carBody->at(i)->Rotate(modelStack, car->wheelAngle, 1, 0, 0);

	//	//}
	//	//else if (i == 2)
	//	//{
	//	//	carBody->at(i)->Translate(modelStack, -1.55f, 0.72f, 2.2f);
	//	//	carBody->at(i)->Rotate(modelStack, -car->steeringWheelAngle, 0, 1, 0);
	//	//	carBody->at(i)->Rotate(modelStack, car->wheelAngle, 1, 0, 0);
	//	//}
	//	//else if (i == 3)
	//	//{
	//	//	carBody->at(i)->Translate(modelStack, 1.55f, 0.73f, -2.85f);
	//	//	carBody->at(i)->Rotate(modelStack, car->wheelAngle, 1, 0, 0);
	//	//}
	//	//else if (i == 4)
	//	//{
	//	//	carBody->at(i)->Translate(modelStack, -1.55f, 0.73f, -2.85f);
	//	//	carBody->at(i)->Rotate(modelStack, car->wheel Angle, 1, 0, 0);
	//	//}
	//	RenderMesh(aiBody->at(i), true);
	//	modelStack.PopMatrix();
	//}


	modelStack.PopMatrix();
	glEnable(GL_CULL_FACE);


}

void SceneA2::RenderUI() 
{
	gui->cursorUpdate(Application::mouse_x, Application::mouse_y);
	gui->renderUI();
	
	gui->renderText("bahnschrift", 0, 40, "Hello World", 0.4f, Color(0, 1, 0));
	gui->renderText("consolas", 0, 400, "Hello World", 0.5f, Color(0, 1, 0));

	gui->renderText("default", 0, 15, "FPS: " + std::to_string(lastFramesPerSecond), 0.4f, Color(0, 1, 0));

	if (!player->isInVehicle && (player->getCar()->position - player->position).Length() <= 6.0f)
		gui->renderText("default", 400, 300, "Press F to enter car", 0.4f, Color(0, 1, 0), TEXT_ALIGN_MIDDLE);

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
	modelStack.Rotate(90.0f, 0, 1, 0);
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
	//if (state_MainMenu)
	//{
	//	Engine.play(Music[BGM_MAIN]);
	//	state_MainMenu = false;
	//}
	//if (state_InGame)
	//{
	//	Engine.play(Music[BGM_INGAME]);
	//	state_InGame = false;
	//}
	//if (!state_Race)
	//{
	//	Engine.play(Music[BGM_RACE]);
	//}

}


void SceneA2::Update(double dt)
{
	// Bounce Time
	playMusic();
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
		else if (Application::IsKeyPressed('F')) {
			if (!player->isInVehicle && (player->getCar()->position - player->position).Length() <= 6.0f) {
				player->isInVehicle = true;
				player->getCar()->setOccupied(true);
				player->setCameraMode(FIXED_CAR);

			}
			else if (player->isInVehicle) {
				player->isInVehicle = false;
				player->getCar()->setOccupied(false);
				player->setCameraMode(FIRST_PERSON);
			}
			bounceTimeCounter = 0.3f;
		}
	
	}

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
	manager->savePlayerProgress(player);
	Engine.deinit();
	delete manager;
	delete gui;

	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(lit->getID());
}

