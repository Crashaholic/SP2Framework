#ifndef OBB_H
#define OBB_H

#include "Vertex.h"

class OBB
{
public:

	OBB(Vector3 half);
	OBB();
	~OBB();
	
	void setPosAxis(Vector3 pos, Vector3 x, Vector3 y, Vector3 z);
	void incrementSize(Vector3 incr);

	Vector3& getPos();
	Vector3& getX();
	Vector3& getY();
	Vector3& getZ();
	Vector3& getHalf();

private:

	Vector3 position;
	Vector3 axisX;
	Vector3 axisY;
	Vector3 axisZ;
	Vector3 halfSize;

};

#endif