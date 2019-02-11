#include "Primitive.h"



Primitive::Primitive()
{

}


Primitive::~Primitive()
{
}

Primitive::Primitive(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{
	Set(vertices, indices);
}

void Primitive::Set(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{
	this->vertices = vertices;
	this->indices = indices;

}

std::vector<Vertex>* Primitive::getVertices()
{
	return &vertices;
}

std::vector<unsigned int>* Primitive::getIndices()
{
	return &indices;
}

