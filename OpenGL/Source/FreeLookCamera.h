#ifndef FPSCamera_H
#define FPSCamera_H


#include "Camera.h"

class FreeLookCamera : public Camera
{
public:

	bool flying;
	bool inCar;

	FreeLookCamera(const Vector3& pos);
	FreeLookCamera();
	~FreeLookCamera();
	Mtx44 LookAt();
	void Init(const Vector3& pos);
	void Reset();
	void Update(double dt);
	void Invert();
	void setFreeLook(bool state);

	Vector3 getRight();
	Vector3 getFront();
	float getYaw();

private:

	bool firstMouse;
	bool canFreeLook;
	double lastX = 0.0;
	double lastY = 0.0;
	float sensitivity = 0.08f;
	float yaw = -90.0f;
	float pitch = 0.0f;

	void updateMouse();
};

#endif