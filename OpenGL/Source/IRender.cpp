#include "IRender.h"

IRender::~IRender()
{
	glDeleteBuffers(1, &vbo);
}

unsigned int IRender::getVBO()
{
	return this->vbo;
}

void IRender::initShape()
{
	glGenBuffers(1, &vbo);
	shape = Primitives::generateQuad(Color(1, 0, 1));
}

void IRender::draw()
{
	glBufferData(GL_ARRAY_BUFFER, shape->getVertices()->size() * sizeof(Vertex), &shape->getVertices()->at(0), GL_STATIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void IRender::setTexture(unsigned int existingTexture)
{
	this->textureID = existingTexture;
}

void IRender::setTexture(const char* newTexture)
{
	this->textureID = LoadTGA(newTexture);
}