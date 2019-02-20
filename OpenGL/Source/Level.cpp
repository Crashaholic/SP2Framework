#include "Level.h"
#include "Manager.h"
#include "Player.h"

Level::Level(const char* levelPath)
{
	//TODO: MAKE A LOADER
	Mtx44 proj;
	proj.SetToPerspective(45.0f, (float)Application::winWidth / (float)Application::winHeight, 0.1f, 10000.0f);
	projectionStack.LoadMatrix(proj);
	
}

void Level::renderGui()
{
	levelGui[0]->render();
}

void Level::renderMesh()
{
	ShaderProgram* lit = Manager::getInstance()->getShader("lit");
	modelStack.LoadIdentity();

	Player* player = dynamic_cast<Player*>(levelMesh["human"]);
	viewStack.LoadMatrix(player->getCamera()->LookAt());



	for (auto& obj : levelMesh) 
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
		Mtx44 MVP, modelView, modelView_inverse_tranpose;
		MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
		modelView = viewStack.Top() * modelStack.Top();

		lit->use();

		bool enableLight = true;
		if (enableLight)
		{
			modelView_inverse_tranpose = modelView.GetInverse().GetTranspose();
			lit->setUniform("lightEnabled", 1);
			lit->setUniform("MV_inverse_transpose", modelView_inverse_tranpose.a);
			lit->setUniform("material.kAmbient", m->material.kAmbient.r, m->material.kAmbient.g, m->material.kAmbient.b);
			lit->setUniform("material.kDiffuse", m->material.kDiffuse.r, m->material.kDiffuse.g, m->material.kDiffuse.b);
			lit->setUniform("material.kSpecular", m->material.kSpecular.r, m->material.kSpecular.g, m->material.kSpecular.b);
			lit->setUniform("material.kShininess", m->material.kShininess);

		}
		else
		{
			lit->setUniform("lightEnabled", 0);
		}

		// Do normal one texture per mesh bind
		if (m->textureID > 0)
		{
			lit->setUniform("colorTextureEnabled", 1);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m->textureID);
			lit->setUniform("colorTexture", 0);
		}
		else
		{
			lit->setUniform("colorTextureEnabled", 0);
		}

		lit->updateUniforms();

		m->Render(modelStack, viewStack, projectionStack, lit);

		if (m->textureID > 0)
			glBindTexture(GL_TEXTURE_2D, 0);
		modelStack.PopMatrix();

		if (obj.first == "ai" || obj.first == "car") {
			glEnable(GL_CULL_FACE);
		}
	}
}

void Level::renderLevel()
{
	glEnable(GL_DEPTH_TEST);
	renderMesh();
	glDisable(GL_DEPTH_TEST);
	renderGui();
}