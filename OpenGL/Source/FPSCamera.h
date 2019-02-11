#ifndef FPSCamera_H
#define FPSCamera_H


#include "Camera.h"

class FPSCamera : public Camera
{
public:
	bool flying;
	bool inCar;

	FPSCamera();
	~FPSCamera();
	Mtx44 LookAt();
	void Init(const Vector3& pos);
	void Reset();
	void Update(double dt);

	void Invert();
	Vector3 getRight();
	Vector3 getFront();
	float getYaw();

private:
	bool firstMouse;
	double lastX = 0.0;
	double lastY = 0.0;
	float sensitivity = 0.08f;
	float yaw = -90.0f;
	float pitch = 0.0f;
	void UpdateMouse();
};

#endif