#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3.h"
#include "Mtx44.h"

class Camera
{
public:
	Camera(const Vector3& pos);
	Camera();
	~Camera();
	Vector3 position;
	virtual Mtx44 LookAt();
	Mtx44 LookAtV2();
	virtual void Init(const Vector3& pos);
	virtual void Reset();
	virtual void Update(double dt);
	void setTarget(const Vector3& pos);

	
protected:
	Vector3 target;
	Vector3 up;
	Vector3 front;


};

#endif



/*
#ifndef FPSCamera_H
#define FPSCamera_H

#include "Vector3.h"
#include "Mtx44.h"


class FreeLookCamera
{
public:

Vector3 position;
bool flying;
bool inCar;

FreeLookCamera();
~FreeLookCamera();
virtual void Init(const Vector3& pos);
virtual void Reset();
virtual void Update(double dt);

Mtx44 LookAt();
void Invert();


private:



Vector3 target;
Vector3 up;
Vector3 front;

bool firstMouse;
double lastX = 0.0;
double lastY = 0.0;
float sensitivity = 0.08f;
float yaw = -90.0f;
float pitch = 0.0f;
void UpdateMouse();
};

#endif
*/