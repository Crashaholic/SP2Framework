#ifndef COLLISION_H
#define COLLISION_H

#include "OBB.h"

class Collision
{
public:
	Collision(); 
	~Collision();
	static bool checkCollision(OBB& box, OBB& other);
private:
	static bool getSeparatingPlane(const Vector3& pos, const Vector3& planeAxis, OBB& box, OBB& other);

};

#endif