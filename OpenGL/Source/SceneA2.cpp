#include "SceneA2.h"
#include "GL/glew.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include "Primitives.h"
#include "shader.hpp"
#include "ShaderProgram.h"
#include "Collision.h"


SceneA2::SceneA2()
{
}


SceneA2::~SceneA2()
{
}


void SceneA2::Init()
{
	manager = Manager::getInstance();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	elapsedTimeCounter = bounceTimeCounter = lastTimed = 0.0f;
	lastFramesPerSecond = framesPerSecond = 1;

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);


	InitShaderProperties();
	CreateMesh();

	Mtx44 projection;
	projection.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
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

	// Environment
	manager->spawnObject(new Mesh("ground", Primitives::loadModel("Models//ground.obj"), LoadTGA("Image//rock.tga"), false));

	Vector3 p = Utility::rotatePointByY(Vector3(0, 0, 1), 90);
	std::cout << p.x << "," << p.y << ","  << p.z << std::endl;

	//OBB a(Vector3(0, 0, 0), Vector3(1, 0, 0), Vector3(0, 1, 0), Vector3(0, 0, 1), Vector3(10, 1, 1));
	//OBB b(Vector3(21, 0, 0), Vector3(1, 0, 0), Vector3(0, 1, 0), Vector3(0, 0, 1), Vector3(10, 1, 1));
	//std::cout << Collision::checkCollision(a, b) << std::endl;
}



void SceneA2::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	modelStack.LoadIdentity();

	viewStack.LoadMatrix(manager->getCamera()->LookAt());

	std::vector<LightSource*>* lightSources = manager->getLightSources();
	for (int i = 0; i < (int)lightSources->size(); i++){
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
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(manager->getObject("ground"), true);
	modelStack.PopMatrix();


}

void SceneA2::RenderUI() {
	gui->renderUI();
	gui->renderText("game", 0, 15, "FPS: " + std::to_string(lastFramesPerSecond), 0.4f, Color(0, 1, 0));
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


void SceneA2::Update(double dt)
{
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

	manager->getCamera()->Update(dt);

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
	delete manager;
	delete gui;

	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(lit->getID());
}

