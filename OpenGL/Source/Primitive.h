#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "Vertex.h"
#include <vector>

class Primitive
{
public:
	Primitive(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Vector3 min, Vector3 max);
	Primitive();
	~Primitive();
	void Set(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Vector3 min, Vector3 max);
	std::vector<Vertex>* getVertices();
	std::vector<unsigned int>* getIndices();
	float getWidth();
	float getHeight();
	float getDepth();

private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	Vector3 min;
	Vector3 max;

};

#endif

