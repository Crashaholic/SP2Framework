#include "QuadTree.h"


QuadTree::QuadTree(Vector3 min, Vector3 max) {
	this->min = min;
	this->max = max;
}

QuadTree::QuadTree() {

}

QuadTree::~QuadTree() {

}

bool QuadTree::withinBounds(Vector3 point) {
	return (point.x >= min.x && point.x <= max.x &&
		point.z >= min.z && point.z <= max.z);
}

bool QuadTree::withinBounds(Vector3 point, Vector3 min, Vector3 max) {
	return (point.x >= min.x && point.x <= max.x &&
		point.z >= min.z && point.z <= max.z);
}

void QuadTree::Subdivide() {
	Vector3 half = 0.5f* (min + max);
	topLeft = new QuadTree(min, half);
	topRight = new QuadTree(Vector3(half.x, 0, min.z), Vector3(max.x, 0, half.z));
	bottomLeft = new QuadTree(Vector3(min.x, 0, half.z), Vector3(half.x, 0, max.z));
	bottomRight = new QuadTree(half, max);
}


bool QuadTree::Insert(Mesh* mesh) {

	Vector3 pos = mesh->position;

	if (!withinBounds(pos)) return false;

	if (meshes.size() < capacity) {
		meshes.push_back(mesh);
		return true;
	}

	// Subdivide and add each point according to the Quad it belongs in
	if (topRight == nullptr) Subdivide();

	if (topRight->Insert(mesh)) return true;
	if (topLeft->Insert(mesh)) return true;
	if (bottomLeft->Insert(mesh)) return true;
	if (bottomRight->Insert(mesh)) return true;

	return false;
}

bool QuadTree::Delete(Mesh* mesh) {

	if(std::find(meshes.begin(), meshes.end(), mesh) != meshes.end()){
		meshes.erase(std::find(meshes.begin(), meshes.end(), mesh));
		//if (meshes.size() == 0) {
		//	if (topRight != nullptr) delete topRight;
		//	if (topLeft != nullptr) delete topLeft;
		//	if (bottomLeft != nullptr) delete bottomLeft;
		//	if (bottomRight != nullptr) delete bottomRight;
		//}
		return true;
	}


	if(topRight != nullptr)
		if (topRight->Delete(mesh)) return true;

	if(topLeft != nullptr)
		if (topLeft->Delete(mesh)) return true;

	if(bottomLeft != nullptr)
		if (bottomLeft->Delete(mesh)) return true;

	if(bottomRight != nullptr)
		if (bottomRight->Delete(mesh)) return true;

	return false;
}

void QuadTree::Update(Mesh* m) {
	
	Delete(m);
	Insert(m);

	//bool QuadTree::update(Collidable *obj) {
	//	if (!remove(obj)) return false;

	//	// Not contained in this node -- insert into parent
	//	if (parent != nullptr && !bounds.contains(obj->bound))
	//		return parent->insert(obj);
	//	if (!isLeaf) {
	//		// Still within current node -- insert into leaf
	//		if (QuadTree *child = getChild(obj->bound))
	//			return child->insert(obj);
	//	}
	//	return insert(obj);
	//}
}

bool QuadTree::placeOverlap(Vector3 minA, Vector3 maxA, Vector3 minB, Vector3 maxB) {
	if (maxA.x < minB.x || minA.x > maxB.x || maxA.z < minB.z || minA.z > maxB.z)
		return false;
	else
		return true;
}

std::vector<Mesh*> QuadTree::queryMesh(Vector3 position, float width, float depth) {
	Vector3 minB = position + Vector3(-width, 0, -depth);
	Vector3 maxB = position + Vector3(width, 0, depth);
	return queryMesh(minB, maxB);
}

std::vector<Mesh*> QuadTree::queryMesh(Vector3 minB, Vector3 maxB) {

	std::vector<Mesh*> results;

	// Ignore if specified bounds doesn't overlap with Quadtree's boundary
	if (!placeOverlap(min, max, minB, maxB))
		return results;

	// For every mesh, check if they are within the specified bounds and add them to the results if they are
	for (int g = 0; g < (int)meshes.size(); g++) {
		Mesh* current = meshes[g];
		if (current == nullptr) continue;
		if (withinBounds(current->position, minB, maxB))
			results.push_back(current);
	}

	// Terminate if there are no children Quads
	if (topRight == nullptr)
		return results;

	// Otherwise, add all the mesh from children Quads
	std::vector<Mesh*> topRightResults = topRight->queryMesh(minB, maxB);
	results.insert(results.end(), topRightResults.begin(), topRightResults.end());

	std::vector<Mesh*> topLeftResults = topLeft->queryMesh(minB, maxB);
	results.insert(results.end(), topLeftResults.begin(), topLeftResults.end());

	std::vector<Mesh*> bottomLeftResults = bottomLeft->queryMesh(minB, maxB);
	results.insert(results.end(), bottomLeftResults.begin(), bottomLeftResults.end());

	std::vector<Mesh*> bottomRightResults = bottomRight->queryMesh(minB, maxB);
	results.insert(results.end(), bottomRightResults.begin(), bottomRightResults.end());

	return results;
}