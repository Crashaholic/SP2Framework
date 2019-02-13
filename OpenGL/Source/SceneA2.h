#ifndef SCENE_A2_H
#define SCENE_A2_H

#define NUM_BUFFERS 3
#define BUFFER_SIZE 4096

#include "Scene.h"
#include "FPSCamera.h"
#include "Light.h"
#include "MatrixStack.h"
#include "Mtx44.h"
#include "Mesh.h"
#include <conio.h>
#include <vector>
#include <string>
#include "LightSource.h"
#include "Manager.h"
#include "GUIManager.h"
#include "al.h"
#include "alc.h"

class SceneA2 : public Scene
{

public:
	SceneA2();
	~SceneA2();

	virtual void Init();
	virtual void Render();
	virtual void Update(double dt);
	virtual void Exit();
private:



	void RenderMesh(Mesh* mesh, bool enableLight, unsigned int shader=0);
	void RenderScene();
	void RenderUI();
	void RenderSkybox();
	void InitShaderProperties();
	void CreateMesh();


	MS modelStack, viewStack, projectionStack;

	unsigned m_vertexArrayID;

	float bounceTimeCounter = 0.0f;
	float lastTimed = 0.0f;
	int framesPerSecond = 0;
	int lastFramesPerSecond = 0;
	float elapsedTimeCounter = 0.0f;

	Manager *manager;
	GUIManager *gui;
	ShaderProgram* lit;

	ALCdevice* deviceOne;
	ALCboolean enumeration;
	ALCcontext* context;

	ALuint buffer[NUM_BUFFERS];
	ALuint source[BUFFER_SIZE];
};

#endif