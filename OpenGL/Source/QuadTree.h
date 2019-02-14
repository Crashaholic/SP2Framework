#ifndef QUADTREE_H
#define QUADTREE_H


#include "QuadNode.h"

class QuadTree
{
public:
	QuadTree(Vector3 topLeft, Vector3 bottomRight);
	QuadTree();
	~QuadTree();
	Vector3 topLeft;
	Vector3 botRight;

	QuadNode* node;

	QuadTree *topLeftTree;
	QuadTree *topRightTree;
	QuadTree *botLeftTree;
	QuadTree *botRightTree;

	void Insert(QuadNode* node);
	QuadNode* Search(Vector3 point);
	bool isWithin(Vector3 point);
};

#endif