#include "AIWaypoint.h"


AIWaypoint::AIWaypoint(Vector3 position, float rot, float accInput, float steerInput, float tolerance) {
	this->position = position;
	this->accInput = accInput;
	this->rotY = rot;
	this->steerInput = steerInput;
	this->tolerance = tolerance;
}

AIWaypoint::AIWaypoint()
{
}


AIWaypoint::~AIWaypoint()
{
}


float AIWaypoint::getAccInput() {
	return accInput;
}

float AIWaypoint::getSteerInput() {
	return steerInput;
}

Vector3 AIWaypoint::getPos() {
	return position;
}

float AIWaypoint::getTolerance() {
	return tolerance;
}

float AIWaypoint::getRot() {
	return rotY;
}