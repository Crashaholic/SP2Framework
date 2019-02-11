#include "CollisionManager.h"



CollisionManager* CollisionManager::instance = nullptr;

CollisionManager::CollisionManager()
{
}


CollisionManager::~CollisionManager()
{

}

CollisionManager* CollisionManager::getInstance()
{
	if (instance == nullptr)
		instance = new CollisionManager();
	return instance;
}


void CollisionManager::addNewCollidable(Mesh* mesh)
{
	collidables.push_back(mesh);
}

/*

Check whether target point lies within any collider other than itself, with the option to ignore the player.
Returns the mesh of the collider in contact, else returns null.

*/

Mesh* CollisionManager::checkCollision(Mesh* self, Vector3 point, bool ignorePlayer)
{

	//for (int i = 0; i < (int)collidables.size(); i++)
	//	if (collidables[i] != player || (!ignorePlayer && collidables[i] == player))
	//		if (collidables[i] != self && collidables[i]->collider.contains(point)) return collidables[i];
	return nullptr;
}

/*

Check whether a collider overlaps with any collider other than itself, with the option to ignore the player.
Returns the mesh of the collider in contact, else returns null.

*/

Mesh* CollisionManager::checkCollision(Mesh* self, bool ignorePlayer)
{

	//for (int i = 0; i < (int)collidables.size(); i++)
	//	if (collidables[i] != player || (!ignorePlayer && collidables[i] == player))
	//		if (collidables[i] != self && collidables[i]->collider.overlaps(bound)) return collidables[i];
	return nullptr;

}