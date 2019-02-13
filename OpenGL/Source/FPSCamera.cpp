#include "FPSCamera.h"
#include "Application.h"


FPSCamera::FPSCamera()
{
	Reset();
}

FPSCamera::~FPSCamera()
{
}

void FPSCamera::Init(const Vector3& pos)
{
	firstMouse = true;
	lastX = 400.0f;
	lastY = 300.0f;
	yaw = -89.0f;
	pitch = 0.0f;
	sensitivity = 0.123591f;

	front = Vector3(0.0f, 0.0f, 1.0f);
	up = Vector3(0.0f, 1.0f, 0.0f);
	this->position = pos;
	
	flying = true;
	inCar = false;

	UpdateMouse();
}

void FPSCamera::Reset()
{
	position.Set(1, 100, 0);
	target.Set(0, 0, 0);
	up.Set(0, 1, 0);
}

void FPSCamera::UpdateMouse()
{
	if (firstMouse)
	{
		lastX = Application::mouse_x;
		lastY = Application::mouse_y;
		firstMouse = false;
	}

	double xoffset = (Application::mouse_x - lastX);
	// Flip since Y-coordinates range from bottom to top
	double yoffset = (lastY - Application::mouse_y);
	lastX = Application::mouse_x;
	lastY = Application::mouse_y;


	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += (float)xoffset;
	pitch += (float)yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	float pitchRad = Math::DegreeToRadian(pitch);
	float yawRad = Math::DegreeToRadian(yaw);
	front.x = cos(pitchRad) * cos(yawRad);
	front.y = sin(pitchRad);
	front.z = cos(pitchRad) * sin(yawRad);
	front.Normalize();


	this->target = position + front;

}

void FPSCamera::Update(double dt)
{


	UpdateMouse();

}

Mtx44 FPSCamera::LookAt()
{
	Vector3 f = front.Normalize();
	Vector3 s = f.Cross(up).Normalize();
	Vector3 u = s.Cross(f);

	Mtx44 mat(s.x, u.x, -f.x, 0,
		s.y, u.y, -f.y, 0,
		s.z, u.z, -f.z, 0,
		-s.Dot(target), -u.Dot(target), f.Dot(target), 1);
	return mat;
}



void FPSCamera::Invert()
{
	pitch = -pitch;
}

Vector3 FPSCamera::getFront() {
	return front;
}

Vector3 FPSCamera::getRight() {
	return front.Cross(up).Normalize();
}

float FPSCamera::getYaw() {
	return yaw;
}