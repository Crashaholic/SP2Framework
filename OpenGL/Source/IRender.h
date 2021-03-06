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
	unsigned int vao;
	unsigned int vbo;

	Vector3 pos, rot, scale;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

public:
	IRender(Vector3 pos, Vector3 rot, Vector3 scale,
		std::vector<Vertex> vertices, std::vector<unsigned> indices,
		unsigned int textureID);
	~IRender();

	virtual void draw();
	virtual void SetPos(Vector3 b);
	virtual void setTexture(unsigned int existingTexture);
	virtual void setTexture(const char* newTexture);
};

#endif
