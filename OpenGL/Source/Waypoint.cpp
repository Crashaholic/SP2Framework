#include "Waypoint.h"


Waypoint::Waypoint(Vector3 position, Vector3 scale) {


	obb = new OBB(scale * 0.5f);
	obb->setPos(position);
}


Waypoint::Waypoint()
{
}


Waypoint::~Waypoint()
{
}


OBB* Waypoint::getOBB() {
	return obb;
}