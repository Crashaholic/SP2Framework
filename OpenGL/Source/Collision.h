#ifndef COLLISION_H
#define COLLISION_H

#include "Mesh.h"
#include <algorithm>

class Collision
{
public:
	Collision(); 
	~Collision();
	static std::vector<Mesh*> checkCollision(Mesh* mesh);
	static std::vector<Mesh*> checkCollisionT(Mesh* mesh, Vector3& translation, std::vector<std::string> exceptions);
	static std::vector<Mesh*> checkCollisionR(Mesh* mesh, Vector3& rotation, std::vector<std::string> exceptions);
	static std::vector<Mesh*> checkCollisionAbove(Mesh* mesh, float distance, std::vector<std::string> exceptions);
	static std::vector<Mesh*> checkCollisionType(Mesh* mesh, Vector3& translation, std::string type);
	static void Collide(float initialVelA, float initialVelB, float massA, float massB, float& finalVelA, float& finalVelB, float percentageLost);
private:
	static bool checkCollision(OBB& box, OBB& other);
	static bool checkCollision(OBB& box, OBB& other, Vector3& translation);
	static bool getSeparatingPlane(const Vector3& pos, const Vector3& planeAxis, OBB& box, OBB& other);

};

#endif