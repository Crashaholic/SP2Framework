#include "Collision.h"
#include "Manager.h"


Collision::Collision()
{
}


Collision::~Collision()
{
}


bool Collision::getSeparatingPlane(const Vector3& pos, const Vector3& planeAxis, OBB& box, OBB& other) {
	return (fabs(pos.Dot(planeAxis)) > 
		(fabs((box.getX() * box.getHalf().x).Dot(planeAxis)) +
		fabs((box.getY() * box.getHalf().y).Dot(planeAxis)) +
		fabs((box.getZ() * box.getHalf().z).Dot(planeAxis)) +
		fabs((other.getX() * other.getHalf().x).Dot(planeAxis)) +
		fabs((other.getY() * other.getHalf().y).Dot(planeAxis)) +
		fabs((other.getZ() * other.getHalf().z).Dot(planeAxis))));
}

bool Collision::checkCollision(OBB& box, OBB& other) {
	Vector3 pos = other.getPos() - box.getPos();
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
