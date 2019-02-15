#ifndef QUADNODE_H
#define QUADNODE_H

#include "OBB.h"

class QuadNode
{
public:
	QuadNode(OBB obb);
	QuadNode(Vector3 pos);
	QuadNode();
	~QuadNode();
	OBB* getOBB();
private:

	OBB obb;
};

#endif