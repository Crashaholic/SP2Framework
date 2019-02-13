#include "Collision.h"
#include "Manager.h"


Collision::Collision()
{
}


Collision::~Collision()
{
}


bool Collision::getSeparatingPlane(const Vector3& pos, const Vector3& planeAxis, OBB& box, OBB& other) {

	float a1 = fabs((box.getX() * box.getHalf().x).Dot(planeAxis));
	float a2 = fabs((box.getY() * box.getHalf().y).Dot(planeAxis));
	float a3 = fabs((box.getZ() * box.getHalf().z).Dot(planeAxis));
	float b1 = fabs((other.getX() * other.getHalf().x).Dot(planeAxis));
	float b2 = fabs((other.getY() * other.getHalf().y).Dot(planeAxis));
	float b3 = fabs((other.getZ() * other.getHalf().z).Dot(planeAxis));

	float result = a1 + a2 + a3 + b1 + b2 + b3;
	float c1 = fabs(pos.Dot(planeAxis));

	return (c1 > result);
}

bool Collision::checkCollision(OBB& box, OBB& other) {

	Vector3 pos = other.getPos() - box.getPos();
	//std::vector<Vector3> separatingAxesNormal;

	//for (int i = 0; i < 15; i++)
	//{
	//	bool result;
	//	Vector3 axis;
	//	if (i == 0)
	//		axis = box.getX();
	//	else if (i == 1)
	//		axis = box.getY();
	//	else if (i == 2)
	//		axis = box.getZ();
	//	else if (i == 3)
	//		axis = other.getX();
	//	else if (i == 4)
	//		axis = other.getY();
	//	else if (i == 5)
	//		axis = other.getZ();
	//	else if (i == 6)
	//		axis = box.getX().Cross(other.getX());
	//	else if (i == 7)
	//		axis = box.getX().Cross(other.getY());
	//	else if (i == 8)
	//		axis = box.getX().Cross(other.getZ());
	//	else if (i == 9)
	//		axis = box.getY().Cross(other.getX());
	//	else if (i == 10)
	//		axis = box.getY().Cross(other.getY());
	//	else if (i == 11)
	//		axis = box.getY().Cross(other.getZ());
	//	else if (i == 12)
	//		axis = box.getZ().Cross(other.getX());
	//	else if (i == 13)
	//		axis = box.getZ().Cross(other.getY());
	//	else if (i == 14)
	//		axis = box.getZ().Cross(other.getZ());

	//	result = getSeparatingPlane(pos, axis, box, other);
	//	if (result) separatingAxesNormal.push_back(axis);
	//}



	//return (separatingAxesNormal.size() == 0);
	
	return !(getSeparatingPlane(pos, box.getX(), box, other) ||
		getSeparatingPlane(pos, box.getY(), box, other) ||
		getSeparatingPlane(pos, box.getZ(), box, other) ||
		getSeparatingPlane(pos, other.getX(), box, other) ||
		getSeparatingPlane(pos, other.getY(), box, other) ||
		getSeparatingPlane(pos, other.getZ(), box, other) ||
		getSeparatingPlane(pos, box.getX().Cross(other.getX()), box, other) ||
		getSeparatingPlane(pos, box.getX().Cross(other.getY()), box, other) ||
		getSeparatingPlane(pos, box.getX().Cross(other.getZ()), box, other) ||
		getSeparatingPlane(pos, box.getY().Cross(other.getX()), box, other) ||
		getSeparatingPlane(pos, box.getY().Cross(other.getY()), box, other) ||
		getSeparatingPlane(pos, box.getY().Cross(other.getZ()), box, other) ||
		getSeparatingPlane(pos, box.getZ().Cross(other.getX()), box, other) ||
		getSeparatingPlane(pos, box.getZ().Cross(other.getY()), box, other) ||
		getSeparatingPlane(pos, box.getZ().Cross(other.getZ()), box, other));
}

bool Collision::checkCollision(OBB& box, OBB& other, Vector3& translation) {

	Vector3 pos = other.getPos() - (box.getPos() + translation);


	//std::vector<Vector3> notSeparatingAxes;

	//for (int i = 0; i < 15; i++)
	//{
	//	bool result;
	//	Vector3 axis;
	//	if (i == 0)
	//		axis = box.getX();
	//	else if (i == 1)
	//		axis = box.getY();
	//	else if (i == 2)
	//		axis = box.getZ();
	//	else if (i == 3)
	//		axis = other.getX();
	//	else if (i == 4)
	//		axis = other.getY();
	//	else if (i == 5)
	//		axis = other.getZ();
	//	else if (i == 6)
	//		axis = box.getX().Cross(other.getX());
	//	else if (i == 7)
	//		axis = box.getX().Cross(other.getY());
	//	else if (i == 8)
	//		axis = box.getX().Cross(other.getZ());
	//	else if (i == 9)
	//		axis = box.getY().Cross(other.getX());
	//	else if (i == 10)
	//		axis = box.getY().Cross(other.getY());
	//	else if (i == 11)
	//		axis = box.getY().Cross(other.getZ());
	//	else if (i == 12)
	//		axis = box.getZ().Cross(other.getX());
	//	else if (i == 13)
	//		axis = box.getZ().Cross(other.getY());
	//	else if (i == 14)
	//		axis = box.getZ().Cross(other.getZ());

	//	result = getSeparatingPlane(pos, axis, box, other);
	//	if (!result) notSeparatingAxes.push_back(axis);
	//}

	//float minimumOverlap;
	//Vector3 minimumAxis;

	//for (int i = 0; i < notSeparatingAxes.size(); i++)
	//{
	//	float a1 = fabs((box.getX() * box.getHalf().x).Dot(notSeparatingAxes[i]));
	//	float a2 = fabs((box.getY() * box.getHalf().y).Dot(notSeparatingAxes[i]));
	//	float a3 = fabs((box.getZ() * box.getHalf().z).Dot(notSeparatingAxes[i]));
	//	float b1 = fabs((other.getX() * other.getHalf().x).Dot(notSeparatingAxes[i]));
	//	float b2 = fabs((other.getY() * other.getHalf().y).Dot(notSeparatingAxes[i]));
	//	float b3 = fabs((other.getZ() * other.getHalf().z).Dot(notSeparatingAxes[i]));

	//	float result = a1 + a2 + a3 + b1 + b2 + b3;
	//	float c1 = fabs(pos.Dot(notSeparatingAxes[i]));
	//	float z = 0;

	//	if (i == 0 || result < minimumOverlap)
	//	{
	//		minimumAxis = notSeparatingAxes[i];
	//		minimumOverlap = result;
	//	}
	//}

 //	return (notSeparatingAxes.size() == 15);
	return !(getSeparatingPlane(pos, box.getX(), box, other) ||
		getSeparatingPlane(pos, box.getY(), box, other) ||
		getSeparatingPlane(pos, box.getZ(), box, other) ||
		getSeparatingPlane(pos, other.getX(), box, other) ||
		getSeparatingPlane(pos, other.getY(), box, other) ||
		getSeparatingPlane(pos, other.getZ(), box, other) ||
		getSeparatingPlane(pos, box.getX().Cross(other.getX()), box, other) ||
		getSeparatingPlane(pos, box.getX().Cross(other.getY()), box, other) ||
		getSeparatingPlane(pos, box.getX().Cross(other.getZ()), box, other) ||
		getSeparatingPlane(pos, box.getY().Cross(other.getX()), box, other) ||
		getSeparatingPlane(pos, box.getY().Cross(other.getY()), box, other) ||
		getSeparatingPlane(pos, box.getY().Cross(other.getZ()), box, other) ||
		getSeparatingPlane(pos, box.getZ().Cross(other.getX()), box, other) ||
		getSeparatingPlane(pos, box.getZ().Cross(other.getY()), box, other) ||
		getSeparatingPlane(pos, box.getZ().Cross(other.getZ()), box, other));
}



//Mesh* Collision::checkCollision(Mesh* mesh, Vector3& translation) {
//	Manager* manager = Manager::getInstance();
//	Vector3 target = mesh->getOBB()->getPos() + translation;
//	std::cout << "Target: " << target << std::endl;
//	std::map<std::string, Mesh*>* objects = manager->getObjects();
//	for (auto const& object : *objects) {
//		// Skip self and collision-disabled objects
//		if (object.second == mesh || !object.second->collisionEnabled) continue;
//
//		bool doesCollide = checkCollision(*mesh->getOBB(), *object.second->getOBB(), target);
//		if (doesCollide)
//			return object.second;
//	}
//	return nullptr;
//}

std::vector<Mesh*> Collision::checkCollision(Mesh* mesh) {
	Manager* manager = Manager::getInstance();
	std::map<std::string, Mesh*>* objects = manager->getObjects();

	std::vector<Mesh*> collided;

	for (auto const& object : *objects) {
		// Skip self and collision-disabled objects
		if (object.second == mesh || !object.second->collisionEnabled) continue;

		bool doesCollide = checkCollision(*mesh->getOBB(), *object.second->getOBB());
		if (doesCollide)
			collided.push_back(object.second);
	}
	return collided;
}

std::vector<Mesh*> Collision::checkCollisionT(Mesh* mesh, Vector3& translation, std::vector<std::string> exceptions)
{
	Manager* manager = Manager::getInstance();
	std::map<std::string, Mesh*>* objects = manager->getObjects();

	std::vector<Mesh*> collided;



	for (auto const& object : *objects)
	{
		// Skip self and collision-disabled objects
		if (object.second == mesh || !object.second->collisionEnabled ||
			std::find(exceptions.begin(), exceptions.end(), object.second->name) != exceptions.end()) continue;

		bool doesCollide = checkCollision(*mesh->getOBB(), *object.second->getOBB(), translation);
		if (doesCollide)
			collided.push_back(object.second);
	}
	return collided;
}


std::vector<Mesh*> Collision::checkCollisionR(Mesh* mesh, Vector3& rotation, std::vector<std::string> exceptions)
{
	Manager* manager = Manager::getInstance();
	std::map<std::string, Mesh*>* objects = manager->getObjects();

	std::vector<Mesh*> collided;

	OBB rotated = mesh->getOBB()->Rotate(rotation);


	for (auto const& object : *objects)
	{
		// Skip self and collision-disabled objects
		if (object.second == mesh || !object.second->collisionEnabled ||
			std::find(exceptions.begin(), exceptions.end(), object.second->name) != exceptions.end()) continue;

		bool doesCollide = checkCollision(rotated, *object.second->getOBB());
		if (doesCollide)
			collided.push_back(object.second);
	}
	return collided;
}
