#include "OBB.h"



OBB::OBB(Vector3 pos, Vector3 axisX, Vector3 axisY, Vector3 axisZ, Vector3 half) {
	this->position = pos;
	this->axisX = axisX;
	this->axisY = axisY;
	this->axisZ = axisZ;
	this->halfSize = half;
}

OBB::OBB()
{
}


OBB::~OBB()
{
}

Vector3& OBB::getPos() {
	return position;
}

Vector3& OBB::getX() {
	return axisX;
}

Vector3& OBB::getY() {
	return axisY;
}

Vector3& OBB::getZ() {
	return axisZ;
}

Vector3& OBB::getHalf() {
	return halfSize;
}