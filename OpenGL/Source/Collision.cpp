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




std::vector<Mesh*> Collision::checkCollision(Mesh* mesh) {
	Manager* manager = Manager::getInstance();
	std::vector<Mesh*> objects = manager->getLevel()->getTree()->queryMesh(mesh->position, 50.0f, 50.0f);
	objects.push_back(manager->getLevel()->getObject("ground"));
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
	std::vector<Mesh*> objects = manager->getLevel()->getTree()->queryMesh(mesh->position, 50.0f, 50.0f);
	objects.push_back(manager->getLevel()->getObject("ground"));
	//std::map<std::string, Mesh*>* objects = manager->getObjects();

	std::vector<Mesh*> collided;

	for(int i = 0; i < objects.size(); i++)
	//for (auto& object : *objects)
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
	std::vector<Mesh*> objects = manager->getLevel()->getTree()->queryMesh(mesh->position, 50.0f, 50.0f);
	objects.push_back(manager->getLevel()->getObject("ground"));
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


std::vector<Mesh*> Collision::checkCollisionAbove(Mesh* mesh, float distance, std::vector<std::string> exceptions)
{
	Manager* manager = Manager::getInstance();
	std::vector<Mesh*> objects = manager->getLevel()->getTree()->queryMesh(mesh->position, 50.0f, 50.0f);
	objects.push_back(manager->getLevel()->getObject("ground"));


	std::vector<Mesh*> collided;

	OBB current = *mesh->getOBB();
	Vector3 pos = current.getPos();
	current.setPos(Vector3(pos.x, pos.y + distance/2.0f, pos.z));
	Vector3 half = current.getHalf();
	current.setHalf(Vector3(half.x, distance/2.0f, half.z));

	for (int i = 0; i < objects.size(); i++)
	{
		Mesh* obj = objects[i];
		// Skip self and collision-disabled objects
		if (obj == mesh || !obj->collisionEnabled ||
			std::find(exceptions.begin(), exceptions.end(), obj->name) != exceptions.end()) continue;

		bool doesCollide = checkCollision(current, *obj->getOBB());
		if (doesCollide)
			collided.push_back(obj);
	}
	return collided;
}

std::vector<Mesh*> Collision::checkCollisionType(Mesh* mesh, Vector3& translation, std::string type) {

	Manager* manager = Manager::getInstance();
	std::vector<Mesh*> objects = manager->getLevel()->getTree()->queryMesh(mesh->position, 50.0f, 50.0f);
	objects.push_back(manager->getLevel()->getObject("ground"));
	//std::map<std::string, Mesh*>* objects = manager->getObjects();

	std::vector<Mesh*> collided;

	for (int i = 0; i < objects.size(); i++)
		//for (auto& object : *objects)
	{
		Mesh* obj = objects[i];
		// Skip self and collision-disabled objects
		if (obj == mesh || !obj->collisionEnabled | obj->getType() != type) continue;

		bool doesCollide = checkCollision(*mesh->getOBB(), *obj->getOBB(), translation);
		if (doesCollide)
			collided.push_back(obj);
	}
	return collided;
}

void Collision::Collide(float initialVelA, float initialVelB, float massA, float massB, float& finalVelA, float& finalVelB, float percentageLost) {

	float percentageRemaining = (100.0f - percentageLost) / 100.0f;

	float y = percentageRemaining * (massA * initialVelA * initialVelA + massB * initialVelB * initialVelB);
	float z = massA * initialVelA + massB * initialVelB;

	float a = massA + massB * pow(massA/massB, 2);
	float b = -2.0f * massB * (z / massB) * (massA / massB);
	float c = massB * pow(z/massB, 2) - y;

	finalVelA = (-b + sqrt(b * b - 4 * a * c)) / (2.0f * a);
	finalVelB = (z - massA * finalVelA) / massB;
}

Mesh* Collision::getNearestObjectType(std::string type, Vector3 position, float distance) {

	std::map<std::string, Mesh*>* objects = Manager::getInstance()->getLevel()->getObjects();
	std::vector<Mesh*> target;
	
	float min = distance;
	Mesh* nearest = nullptr;

	for (auto& object : *objects) {
		if (object.second->getType() == type) {
			float d = (object.second->position - position).Length();
			if (d <= distance) {
				if (d < min) {
					nearest = object.second;
					min = d;
				}
			}
		}
	}

	return nearest;
}