#include "Transformation.h"


Transformation::Transformation(char type, float valX, float valY, float valZ,
	float axisX, float axisY, float axisZ) {

	this->type = type;
	values.Set(valX, valY, valZ);
	axis.Set(axisX, axisY, axisZ);
}


Transformation::Transformation()
{
}


Transformation::~Transformation()
{
}

