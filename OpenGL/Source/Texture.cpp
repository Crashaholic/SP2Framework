#include "Texture.h"
#include "LoadTGA.h"



Texture::Texture(Vector3 position, float rotation, Vector3 scale, const char* filePath, int dontFlip)
{
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;
	this->filePath = filePath;
	this->dontFlip = dontFlip;
	id = LoadTGA(filePath);
}

Texture::Texture(Vector3 position, float rotation, Vector3 scale, unsigned int id, int dontFlip)
{
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;
	this->id = id;
	this->dontFlip = dontFlip;
}

Texture::Texture()
{

}


Texture::~Texture()
{
}

Vector3 Texture::getPosition()
{
	return position;
}

Vector3 Texture::getScale()
{
	return scale;
}

void Texture::setRotation(float f) {
	rotation = f;
}


/*

Performs transformations on the texture before passing the resulting
matrix into the vertex shader to be drawn.

Transformations Order (First To Last):
-> Scale
-> Rotation
-> Translate

*/

Mtx44 Texture::getTransformationMatrix()
{
	Mtx44 rot;
	rot.SetToRotation(rotation, 0, 0, 1);

	Mtx44 matrix;
	matrix.SetToTranslation(position.x, position.y, 0.0f);
	
	matrix.a[0] = scale.x;
	matrix.a[5] = scale.y;

	return matrix * rot;
}

unsigned int Texture::getID()
{
	return id;
}


int Texture::getFlip() {
	return dontFlip;
}
