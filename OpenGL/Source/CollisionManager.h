#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include "Mesh.h"
#include <vector>

class CollisionManager
{
public:
	~CollisionManager();
	static CollisionManager* getInstance();

	void addNewCollidable(Mesh* mesh);
	Mesh* checkCollision(Mesh* self, Vector3 point, bool ignorePlayer=false);
	Mesh* checkCollision(Mesh* self, bool ignorePlayer = false);
private:
	CollisionManager();
	static CollisionManager* instance;
	std::vector<Mesh*> collidables;
};

#endif