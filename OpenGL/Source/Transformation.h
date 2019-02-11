#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include "Vector3.h"

class Transformation
{
public:
	Transformation(char type, float valX, float valY, float valZ, float axisX = 0, float axisY = 0, float axisZ = 0);
	Transformation();
	~Transformation();
	Vector3 values;
	Vector3 axis;
	char type;

};

#endif