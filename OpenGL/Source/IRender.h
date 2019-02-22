#ifndef INTERFACE_RENDER
#define INTERFACE_RENDER

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "LoadTGA.h"
#include "Primitive.h"
#include "Primitives.h"
#include "Application.h"

class IRender
{
private:
	unsigned int textureID;
	unsigned int vao;
	unsigned int vbo;

	Vector3 pos, scale;
	Vector3 color;
	float rot;
	float alpha;
	bool isSolidColor;

public:
	IRender(Vector3 pos, float rot, Vector3 scale,
		unsigned int textureID);
	IRender(Vector3 pos, float rot, Vector3 scale,
		Vector3 color, float alpha);
	~IRender();

	virtual void draw();
	virtual void setPos(Vector3 b);
	virtual void setTexture(unsigned int existingTexture);
	virtual void setTexture(const char* newTexture);
	virtual void setColor(Vector3 color, float alpha);
};

#endif
