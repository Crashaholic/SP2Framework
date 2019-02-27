#ifndef LIGHT_H
#define LIGHT_H

#include "Vertex.h"

struct Light {

	enum LIGHT_TYPE {
		LIGHT_DIRECTIONAL = 0,
		LIGHT_POINT = 1,
		LIGHT_SPOT,

	};

	LIGHT_TYPE type;
	Vector3 spotDirection;
	float cosCutoff;
	float cosInner;
	float exponent;

	Position position;
	Color color; 
	float power;
	float kC, kL, kQ;

	Light() {
		color.Set(1, 1, 1);
		power = 1.0f;
		kC = 1.0f;
		kL = 0.0f;
		kQ = 0.0f;
		type = LIGHT_POINT;
		spotDirection.Set(0.0f, 1.0f, 0.0f);
		exponent = 3.0f;
		cosCutoff = cos(Math::DegreeToRadian(45));
		cosInner = cos(Math::DegreeToRadian(30));
	}


};
#endif