#include "OBB.h"



OBB::OBB(Vector3 half){ 
	axisX = Vector3(1, 0, 0);
	axisY = Vector3(0, 1, 0);
	axisZ = Vector3(0, 0, 1);
	this->halfSize = half;
}

OBB::OBB()
{
}


OBB::~OBB()
{
}

void OBB::incrementSize(Vector3 incr) {
	halfSize.x += incr.x;
	halfSize.y += incr.y;
	halfSize.z += incr.z;
}

void OBB::setPosAxis(Vector3 pos, Vector3 x, Vector3 y, Vector3 z) {
	position = pos;
	axisX = x;
	axisY = y;
	axisZ = z;
}

void OBB::setPos(Vector3 pos) {
	this->position = pos;
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