#include "Manager.h"
#include "IRender.h"
#include "Application.h"
#include "Utility.h"


float quadVertices[] =
{
	-1.0f,  1.0f,
	-1.0f, -1.0f,
	1.0f,  1.0f,
	1.0f, -1.0f,
};

IRender::IRender(Vector3 pos, float rot, Vector3 scale, unsigned int textureID)
{
	glGenBuffers(1, &this->vbo);
	glGenVertexArrays(1, &this->vao);
	this->pos = pos;
	this->rot = rot;
	this->scale = scale;
	this->textureID = textureID;
	this->isSolidColor = false;
}

IRender::IRender(Vector3 pos, float rot, Vector3 scale, Vector3 color, float alpha)
{
	glGenBuffers(1, &this->vbo);
	glGenVertexArrays(1, &this->vao);
	this->pos = pos;
	this->rot = rot;
	this->scale = scale;
	this->color = color;
	this->alpha = alpha;
	this->isSolidColor = true;
}


IRender::~IRender()
{
	glDeleteBuffers(1, &this->vbo);
	glDeleteTextures(1, &this->textureID);
}

void IRender::draw()
{

	/*glBindVertexArray(this->vao);*/

	float SCRWIDTH = (float)Application::winWidth;
	float SCRHEIGHT = (float)Application::winHeight;



	Mtx44 model, view, proj;

	ShaderProgram* shader = Manager::getInstance()->getShader("overlay");
	shader->use();

	Mtx44 transformationMat, translate, rotation, scale;
	translate.SetToTranslation(pos.x, pos.y, 0.0f);
	rotation.SetToRotation(rot, 0, 0, 1);
	scale.SetToScale(this->scale.x, this->scale.y, 1.0f);
	model = transformationMat =  translate * rotation * scale;
	view.SetToIdentity();
	proj.SetToOrtho(
		-SCRWIDTH / 2 , SCRWIDTH / 2 ,
		-SCRHEIGHT /2 , SCRHEIGHT/ 2 ,
		-10.0f, 10.0f);

	Mtx44 mvp = proj * view * model;

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(float) * 2, (void*)0);

	shader->setUniform("mvp", mvp.a);
	shader->setUniform("Flip", 0);
	shader->setUniform("transformationMatrix", transformationMat.a);
	shader->setUniform("colorTexture", 0);
	if (isSolidColor)
	{
		shader->setUniform("elementColor", this->color);
		shader->setUniform("alpha", this->alpha);
		shader->setUniform("useSolidColor", this->isSolidColor);
	}
	else
	{
		shader->setUniform("elementColor", Vector3(1, 1, 1));
		shader->setUniform("alpha", 1);
		shader->setUniform("useSolidColor", this->isSolidColor);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->textureID);
	}

	shader->updateUniforms();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	/*glBindVertexArray(0);*/
	glDisableVertexAttribArray(0);

}

void IRender::setPos(Vector3 b)
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

void IRender::setColor(Vector3 color, float alpha)
{
	this->color = color;
	this->alpha = alpha;
}