#ifndef INTERFACE_RENDER
#define INTERFACE_RENDER

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "LoadTGA.h"
#include "Primitive.h"
#include "Primitives.h"

class IRender
{
private:
	unsigned int textureID;
	unsigned int vbo;
	Primitive* shape;

public:
	IRender() {}
	~IRender();

	virtual unsigned int getVBO();
	virtual void initShape();
	virtual void draw();
	virtual void setTexture(unsigned int existingTexture);
	virtual void setTexture(const char* newTexture);
};

#endif