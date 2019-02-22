#ifndef QUADTREE_H
#define QUADTREE_H

#include "Mesh.h"

class QuadTree
{
public:

	const int capacity = 4;

	float halfSize = 0.0f;
	Vector3 min;
	Vector3 max;
	//OBB boundary;
	std::vector<Mesh*> meshes;

	QuadTree* topLeft = NULL;
	QuadTree* topRight = NULL;
	QuadTree* bottomLeft = NULL;
	QuadTree* bottomRight = NULL;

	bool Insert(Mesh* mesh);
	bool Insert(Mesh* mesh, Vector3 pos);
	bool Delete(Mesh* mesh);
	void Update(Mesh* mesh);
	void Subdivide();
	bool withinBounds(Vector3 point);
	bool withinBounds(Vector3 point, Vector3 min, Vector3 max);
	bool placeOverlap(Vector3 minA, Vector3 maxA, Vector3 minB, Vector3 maxB);
	std::vector<Mesh*> queryMesh(Vector3 position, float width, float depth);
	std::vector<Mesh*> queryMesh(Vector3 bottomLeft, Vector3 topRight);
	QuadTree(Vector3 min, Vector3 max);
	QuadTree();
	~QuadTree();


};


#endif