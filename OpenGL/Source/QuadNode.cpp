#include "QuadNode.h"


QuadNode::QuadNode(OBB obb)
{
	this->obb = obb;
}

QuadNode::QuadNode(Vector3 pos)
{

}

QuadNode::QuadNode()
{
}


QuadNode::~QuadNode()
{
}

OBB* QuadNode::getOBB()
{
	return &obb;
}