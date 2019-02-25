#ifndef WAYPOINT_H
#define WAYPOINT_H

#include "OBB.h"

class Waypoint
{
public:
	Waypoint(Vector3 position, Vector3 scale);
	Waypoint();
	~Waypoint();
	OBB* getOBB();
private:
	OBB* obb;
};

#endif