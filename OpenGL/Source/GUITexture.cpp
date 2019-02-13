#include "GUITexture.h"

GUITexture::GUITexture()
{
	render.initShape();
}

GUITexture::~GUITexture()
{

}

GUITexture::GUITexture(unsigned int existingTexture)
{
	render.setTexture(existingTexture);
}

GUITexture::GUITexture(const char* path)
{
	render.setTexture(path);
}

void GUITexture::setTexture(unsigned int existingTexture)
{
	render.setTexture(existingTexture);
}

void GUITexture::setTexture(const char* newTexture)
{
	render.setTexture(newTexture);
}

void GUITexture::draw(float x, float y, float rotX, float rotY, float scaleX, float scaleY)
{
	glBindBuffer(GL_ARRAY_BUFFER, render.getVBO());

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), (void*) sizeof(Position));
	glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color)));
	glVertexAttribPointer(3, 2, GL_FLOAT, false, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color) + sizeof(Vector3)));

	render.draw();

}