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
	std::vector<Mesh*> objects = manager->getTree()->queryMesh(mesh->position, 50.0f, 50.0f);
	objects.push_back(manager->getObject("ground"));
	//std::map<std::string, Mesh*>* objects = manager->getObjects();
	
	std::vector<Mesh*> collided;

	for (int i = 0; i < objects.size(); i++) {
		// Skip self and collision-disabled objects
		Mesh* obj = objects[i];
		if (obj == mesh || !obj->collisionEnabled) continue;

		bool doesCollide = checkCollision(*mesh->getOBB(), *obj->getOBB());
		if (doesCollide)
			collided.push_back(obj);
	}
	return collided;
}

std::vector<Mesh*> Collision::checkCollisionT(Mesh* mesh, Vector3& translation, std::vector<std::string> exceptions)
{
	Manager* manager = Manager::getInstance();
	std::vector<Mesh*> objects = manager->getTree()->queryMesh(mesh->position, 50.0f, 50.0f);
	objects.push_back(manager->getObject("ground"));
	//std::map<std::string, Mesh*>* objects = manager->getObjects();

	std::vector<Mesh*> collided;



	for (int i = 0; i < objects.size(); i++)
	{
		Mesh* obj = objects[i];
		// Skip self and collision-disabled objects
		if (obj == mesh || !obj->collisionEnabled ||
			std::find(exceptions.begin(), exceptions.end(), obj->name) != exceptions.end()) continue;

		bool doesCollide = checkCollision(*mesh->getOBB(), *obj->getOBB(), translation);
		if (doesCollide)
			collided.push_back(obj);
	}
	return collided;
}


std::vector<Mesh*> Collision::checkCollisionR(Mesh* mesh, Vector3& rotation, std::vector<std::string> exceptions)
{
	Manager* manager = Manager::getInstance();
	std::vector<Mesh*> objects = manager->getTree()->queryMesh(mesh->position, 50.0f, 50.0f);
	objects.push_back(manager->getObject("ground"));
	//std::map<std::string, Mesh*>* objects = manager->getObjects();

	std::vector<Mesh*> collided;

	OBB rotated = mesh->getOBB()->Rotate(rotation);


	for (int i = 0; i < objects.size(); i++)
	{
		Mesh* obj = objects[i];
		// Skip self and collision-disabled objects
		if (obj == mesh || !obj->collisionEnabled ||
			std::find(exceptions.begin(), exceptions.end(), obj->name) != exceptions.end()) continue;

		bool doesCollide = checkCollision(rotated, *obj->getOBB());
		if (doesCollide)
			collided.push_back(obj);
	}
	return collided;
}
