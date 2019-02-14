#include "OctCube.h"



OctCube::OctCube()
{
	bottomLeft = topRight = Vector3(0, 0, 0);
	for (int i = 0; i < 8; i++)
		children[i] = nullptr;
}


OctCube::~OctCube()
{
}

void OctCube::insert(OctNode* node) {
	if (node == nullptr) return;

	if (!isWithin(node->pos)) {
		return;
	}


}