#include "Manager.h"
#include "IRender.h"
#include "Application.h"
#include "Utility.h"

IRender::IRender(Vector3 pos, Vector3 rot, Vector3 scale, 
	std::vector<Vertex> vertices, std::vector<unsigned> indices, unsigned int textureID)
{
	glGenBuffers(1, &this->vbo);
	glGenVertexArrays(1, &this->vao);
	this->pos = pos;
	this->rot = rot;
	this->scale = scale;
	this->vertices = vertices;
	this->indices = indices;
	this->textureID = textureID;
}

IRender::~IRender()
{
	glDeleteBuffers(1, &this->vbo);
	glDeleteTextures(1, &this->textureID);
}

void IRender::draw()
{
	glDisable(GL_DEPTH_TEST);

	glBindVertexArray(this->vao);

	float SCRWIDTH = Application::winWidth;
	float SCRHEIGHT = Application::winHeight;

	float quadVertices[] = {
		-1.0f,  1.0f,
		-1.0f, -1.0f,
		 1.0f,  1.0f,
		 1.0f, -1.0f,
	};

	Mtx44 model, view, proj;

	ShaderProgram* shader = Manager::getInstance()->getShader("overlay");
	shader->use();

	float mouseSensX = 1.5f;
	float mouseSensY = 1.25f;

	Mtx44 transformationMat, translate, rotation, scale;
	float newX = ((pos.x * 100.0f / SCRWIDTH / 2.0f) + 1.0f) * 0.1f * mouseSensX;
	float newY = ((pos.y * 100.0f / SCRHEIGHT / 2.0f) + 1.0f) * 0.1f * mouseSensY;
	translate.SetToTranslation(newX, newY, 0.0f);
	rotation.SetToRotation(rot.z, 0, 0, 1);
	scale.SetToScale(this->scale.x, this->scale.y, 1.0f);
	model = transformationMat =  translate * rotation * scale;
	view.SetToIdentity();
	double aspectRatio = (double)SCRWIDTH / (double)SCRHEIGHT;
	unsigned int percentageIncreaseX = (SCRWIDTH - 20) / 20;
	unsigned int percentageIncreaseY = (SCRHEIGHT - 20) / 20;

	proj.SetToOrtho(
		-SCRWIDTH / 2 / 100, SCRWIDTH / 2 / 100,
		-SCRHEIGHT /2 / 100, SCRHEIGHT/ 2 / 100,
		-10.0f, 10.0f);


	Mtx44 mvp = proj * view * model;

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(float) * 2, (void*)0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->textureID);
	shader->setUniform("mvp", mvp.a);
	shader->setUniform("colorTexture", 0);
	shader->setUniform("Flip", 0);
	shader->setUniform("transformationMatrix", transformationMat.a);
	shader->updateUniforms();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glEnable(GL_DEPTH_TEST);

}

void IRender::SetPos(Vector3 b)
{
	this->pos = b;
}

void IRender::setTexture(unsigned int existingTexture)
{
	this->textureID = existingTexture;
}

void IRender::setTexture(const char* newTexture)
{
	this->textureID = LoadTGA(newTexture);
}