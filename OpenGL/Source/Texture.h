#ifndef TEXTURE_H
#define TEXTURE_H

#include "Vector3.h"
#include "Mtx44.h"

class Texture
{
public:
	Texture(Vector3 position, float rotation, Vector3 scale, unsigned int id, int dontFlip);
	Texture(Vector3 position, float rotation, Vector3 scale, const char *filePath, int dontFlip);
	Texture();
	~Texture();
	Vector3 getPosition();
	Vector3 getScale();
	Mtx44 getTransformationMatrix();
	unsigned int getID();
	int getFlip();
	void setRotation(float f);
private:
	Vector3 position;
	float rotation;
	Vector3 scale;
	int dontFlip;
	unsigned int id;
	const char* filePath;
};

#endif