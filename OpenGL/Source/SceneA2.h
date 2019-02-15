#ifndef SCENE_A2_H
#define SCENE_A2_H


#include "Scene.h"
#include "FreeLookCamera.h"
#include "Light.h"
#include "MatrixStack.h"
#include "Mtx44.h"
#include "Mesh.h"
#include <conio.h>
#include <vector>
#include <string>
#include <Windows.h>
#include "LightSource.h"
#include "Manager.h"
#include "GUIManager.h"
#include "Player.h"

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



	void RenderMesh(Mesh* mesh, bool enableLight, unsigned int shader = 0);
	void RenderScene();
	void RenderUI();
	void RenderSkybox();
	void InitShaderProperties();
	void playMusic();
	void CreateMesh();


	MS modelStack, viewStack, projectionStack;

	unsigned m_vertexArrayID;

	float bounceTimeCounter = 0.0f;
	float lastTimed = 0.0f;
	int framesPerSecond = 0;
	int lastFramesPerSecond = 0;
	float elapsedTimeCounter = 0.0f;

	bool musicFlag = false;

	Manager *manager;
	Player* player;
	Car* car;
	GUIManager *gui;
	ShaderProgram* lit;

};

#endif