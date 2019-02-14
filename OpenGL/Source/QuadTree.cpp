#include "QuadTree.h"


QuadTree::QuadTree(Vector3 topLeft, Vector3 bottomRight)
{
	node = nullptr;
	topLeftTree = topRightTree = botLeftTree = botRightTree = nullptr;
	this->topLeft = topLeft;
	this->botRight = bottomRight;
	
}

QuadTree::QuadTree()
{
	node = nullptr;
	topLeftTree = topRightTree = botLeftTree = botRightTree = nullptr;
	topLeft = botRight = Vector3(0, 0, 0);
}


QuadTree::~QuadTree()
{
}


void QuadTree::Insert(QuadNode* node)
{
	if (node == nullptr) return;

	Vector3 nodePos = node->getOBB()->getPos();

	if (!isWithin(nodePos))
		return;

	if (abs(topLeft.x - botRight.x) <= 1 &&
		abs(topLeft.z - botRight.z) <= 1)
	{
		if (this->node == nullptr)
			this->node = node;
		return;
	}

	if ((topLeft.x + botRight.x) / 2 >= nodePos.x)
	{
		// Indicates topLeftTree 
		if ((topLeft.z + botRight.z) / 2 >= nodePos.z)
		{
			if (topLeftTree == nullptr)
				topLeftTree = new QuadTree(
					Vector3(topLeft.x, 0, topLeft.z),
					Vector3((topLeft.x + botRight.x) / 2, 0,
					(topLeft.z + botRight.z) / 2));
			topLeftTree->Insert(node);
		}

		// Indicates botLeftTree 
		else
		{
			if (botLeftTree == nullptr)
				botLeftTree = new QuadTree(
					Vector3(topLeft.x, 0,
					(topLeft.z + botRight.z) / 2),
					Vector3((topLeft.x + botRight.x) / 2,
						botRight.z));
			botLeftTree->Insert(node);
		}
	}
	else
	{
		// Indicates topRightTree 
		if ((topLeft.z + botRight.z) / 2 >= nodePos.z)
		{
			if (topRightTree == nullptr)
				topRightTree = new QuadTree(
					Vector3((topLeft.x + botRight.x) / 2,
						topLeft.z),
					Vector3(botRight.x,
					(topLeft.z + botRight.z) / 2));
			topRightTree->Insert(node);
		}

		// Indicates botRightTree 
		else
		{
			if (botRightTree == nullptr)
				botRightTree = new QuadTree(
					Vector3((topLeft.x + botRight.x) / 2,
					(topLeft.z + botRight.z) / 2),
					Vector3(botRight.x, botRight.z));
			botRightTree->Insert(node);
		}
	}
}


QuadNode* QuadTree::Search(Vector3 point)
{
	// Current quad cannot contain it 
	if (!isWithin(point))
		return nullptr;

	// We are at a quad of unit length 
	// We cannot subdivide this quad further 
	if (this->node != nullptr)
		return this->node;

	if ((topLeft.x + botRight.x) / 2 >= point.x)
	{
		// Indicates topLeftTree 
		if ((topLeft.z + botRight.z) / 2 >= point.z)
		{
			if (topLeftTree == nullptr)
				return nullptr;
			return topLeftTree->Search(point);
		}

		// Indicates botLeftTree 
		else
		{
			if (botLeftTree == nullptr)
				return nullptr;
			return botLeftTree->Search(point);
		}
	}
	else
	{
		// Indicates topRightTree 
		if ((topLeft.z + botRight.z) / 2 >= point.z)
		{
			if (topRightTree == nullptr)
				return nullptr;
			return topRightTree->Search(point);
		}

		// Indicates botRightTree 
		else
		{
			if (botRightTree == nullptr)
				return nullptr;
			return botRightTree->Search(point);
		}
	}
}

bool QuadTree::isWithin(Vector3 point)
{
	return (point.x >= topLeft.x &&
		point.x <= botRight.x &&
		point.z >= topLeft.z &&
		point.z <= botRight.z);
}

