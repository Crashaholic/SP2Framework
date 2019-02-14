#ifndef OCTCUBE_H
#define OCTCUBE_H

#include "Vector3.h"
#include "OctNode.h"

class OctCube
{
public:
	OctCube();
	~OctCube();
	Vector3 bottomLeft;
	Vector3 topRight;

	OctNode* node;
	OctCube* children[8];

	void insert(OctNode* node);
	OctNode* search(Vector3 point);
	bool isWithin(Vector3 point);


};

#endif