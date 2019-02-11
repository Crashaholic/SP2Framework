#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "Vertex.h"
#include <vector>

class Primitive
{
public:
	Primitive(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	Primitive();
	~Primitive();
	void Set(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	std::vector<Vertex>* getVertices();
	std::vector<unsigned int>* getIndices();

private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

};

#endif

