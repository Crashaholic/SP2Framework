#include "Primitive.h"





Primitive::Primitive()
{

}


Primitive::~Primitive()
{
}

Primitive::Primitive(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Vector3 min, Vector3 max)
{
	Set(vertices, indices, min, max);
}

void Primitive::Set(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Vector3 min, Vector3 max)
{
	this->vertices = vertices;
	this->indices = indices;
	this->min = min;
	this->max = max;

}

std::vector<Vertex>* Primitive::getVertices()
{
	return &vertices;
}

std::vector<unsigned int>* Primitive::getIndices()
{
	return &indices;
}

float Primitive::getWidth() {
	return max.x - min.x;
}

float Primitive::getHeight() {
	return max.y - min.y;
}

float Primitive::getDepth() {
	return max.z - min.z;
}

