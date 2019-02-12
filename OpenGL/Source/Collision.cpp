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

Mesh* Collision::checkCollision(Mesh* mesh, Vector3& translation) {
	Manager* manager = Manager::getInstance();
	Vector3 target = mesh->getOBB()->getPos() + translation;
	
	std::map<std::string, Mesh*>* objects = manager->getObjects();
	for (auto const& object : *objects) {
		// Skip self and collision-disabled objects
		if (object.second == mesh || !object.second->collisionEnabled) continue;

		bool doesCollide = checkCollision(*mesh->getOBB(), *object.second->getOBB());
		if (doesCollide)
			return object.second;
	}
	return nullptr;
}