#ifndef COLLISION_H
#define COLLISION_H

#include "Mesh.h"

class Collision
{
public:
	Collision(); 
	~Collision();
	static bool checkCollision(OBB& box, OBB& other);
	static Mesh* checkCollision(Mesh* mesh, Vector3& translation);
private:
	static bool getSeparatingPlane(const Vector3& pos, const Vector3& planeAxis, OBB& box, OBB& other);

};

#endif