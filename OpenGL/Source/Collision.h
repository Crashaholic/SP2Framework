#ifndef COLLISION_H
#define COLLISION_H

#include "Mesh.h"
#include <algorithm>

class Collision
{
public:
	Collision(); 
	~Collision();
	static Vector3 getMTV(OBB& box, OBB& target);
	static std::vector<Mesh*> checkCollision(Mesh* mesh);
	static std::vector<Mesh*> checkCollisionT(Mesh* mesh, Vector3& translation, std::vector<std::string> exceptions);
	static std::vector<Mesh*> checkCollisionR(Mesh* mesh, Vector3& rotation, std::vector<std::string> exceptions);
	static std::vector<Mesh*> checkCollisionAbove(Mesh* mesh, float distance, std::vector<std::string> exceptions);
	static std::vector<Mesh*> checkCollisionType(Mesh* mesh, Vector3& translation, std::string type);
	static std::vector<Mesh*> checkCollisionTypes(Mesh* mesh, Vector3& translation, std::vector<std::string> types);
	static Mesh* getNearestObjectType(std::string type, Vector3 position, float distance);
	static void Collide(float initialVelA, float initialVelB, float massA, float massB, float& finalVelA, float& finalVelB, float percentageLost);
private:
	static bool checkCollision(OBB& box, OBB& other);
	static bool checkCollision(OBB& box, OBB& other, Vector3& translation);
	static bool getSeparatingPlane(const Vector3& pos, const Vector3& planeAxis, OBB& box, OBB& other);
	static Vector3 getProjection(OBB& box, const Vector3& planeAxis);

};

#endif