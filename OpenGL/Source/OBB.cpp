#include "OBB.h"
#include "Utility.h"


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

OBB OBB::Rotate(Vector3 rotation)
{
	OBB rotated = *this;
	
	// Rotate X
	rotated.axisX = Utility::rotatePointByX(rotated.axisX, rotation.x);
	rotated.axisY = Utility::rotatePointByX(rotated.axisY, rotation.x);
	rotated.axisZ = Utility::rotatePointByX(rotated.axisZ, rotation.x);

	// Rotate Y
	rotated.axisX = Utility::rotatePointByY(rotated.axisX, rotation.y);
	rotated.axisY = Utility::rotatePointByY(rotated.axisY, rotation.y);
	rotated.axisZ = Utility::rotatePointByY(rotated.axisZ, rotation.y);

	// Rotate Z
	rotated.axisX = Utility::rotatePointByZ(rotated.axisX, rotation.z);
	rotated.axisY = Utility::rotatePointByZ(rotated.axisY, rotation.z);
	rotated.axisZ = Utility::rotatePointByZ(rotated.axisZ, rotation.z);

	return rotated;
}

OBB OBB::Translate(Vector3 translation)
{
	OBB translated = *this;
	translated.position += translation;
	return translated;
}

void OBB::setPosAxis(Vector3 pos, Vector3 x, Vector3 y, Vector3 z) {
	position = pos;
	axisX = x;
	axisY = y;
	axisZ = z;
}

void OBB::setHalf(Vector3 half) {
	this->halfSize = half;
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