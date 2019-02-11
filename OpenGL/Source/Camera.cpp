#include "Camera.h"
#include "Application.h"


Camera::Camera()
{
	Reset();
}


Camera::~Camera()
{
}




void Camera::Init(const Vector3& pos)
{
	front = Vector3(0.0f, 0.0f, 1.0f);
	up = Vector3(0.0f, 1.0f, 0.0f);
	this->position = pos;

}

void Camera::setTarget(const Vector3& pos) {
	target = pos;
	front = target - position;
}

void Camera::Reset()
{
	position.Set(1, 0, 0);
	target.Set(0, 0, 0);
	up.Set(0, 1, 0);
}

void Camera::Update(double dt)
{
	
}

Mtx44 Camera::LookAt()
{

	Mtx44 mat;
	mat.SetToLookAt(position.x, position.y, position.z,
		target.x, target.y, target.z, up.x, up.y, up.z);

	return mat;
}

Mtx44 Camera::LookAtV2() {
	Vector3 f = front;
	Vector3 s = f.Cross(up);
	Vector3 u = s.Cross(f);

	Mtx44 mat(s.x, u.x, -f.x, 0,
		s.y, u.y, -f.y, 0,
		s.z, u.z, -f.z, 0,
		0, 0, 0, 1);

	Mtx44 tran;
	tran.SetToTranslation(-(float)position.x, -(float)position.y, -(float)position.z);
	mat = mat * tran;

	
	//mat.SetToRotation(90, 1, 0, 0);

	return mat;
}
