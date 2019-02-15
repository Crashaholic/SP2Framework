#ifndef AIWAYPOINT_H
#define AIWAYPOINT_H

#include "Vector3.h"

/*

Each AIWaypoint stores the state of the car at each given instant.

That includes:
-> Whether the car is accelerating
-> Whether the car is steering

*/

class AIWaypoint
{
public:
	AIWaypoint(Vector3 position, float rotY, float accInput, float steerInput, float tolerance = 0.2f);
	AIWaypoint();
	~AIWaypoint();

	float getAccInput();
	float getTolerance();
	float getSteerInput();
	float getRot();
	Vector3 getPos();


private:
	float rotY;
	float accInput;
	float steerInput;
	Vector3 position;
	float tolerance;
};

#endif