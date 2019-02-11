#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "Vertex.h"
#include <map>
#include <iterator>
#include <vector>
#include <fstream>

#include "Primitive.h"

class Primitives
{

public:


	static Primitive* generateQuad(Color color);
	static Primitive* generateAxes();
	static Primitive* loadModel(const char* path, bool generateCollider=true);
	
	Primitives();
	~Primitives();


};

#endif