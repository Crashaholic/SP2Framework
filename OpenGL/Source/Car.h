#ifndef CAR_H
#define CAR_H

#include "Vector3.h"
#include "Mesh.h"
#include "LightSource.h"

class Car : public Mesh
{
public:
	Car(const char* meshName, Primitive* primitive, unsigned int texID = 0, DRAW_MODE drawMode = DRAW_TRIANGLES);
	Car();
	~Car();

	void Update(double dt);
	void Render(MS& modelStack, MS& viewStack, MS& projectionStack, ShaderProgram* shader);

	float steeringWheelAngle;
	float wheelAngle;
	float steerAngle;
	float currentSteer;
	Vector3 velocity;
	Vector3 target;
	bool showLight;
private:

	float sign(float n);
	Vector3 lerp(Vector3 start, Vector3 end, float percent);
	float clamp(float min, float max, float n);
	float lerp(float start, float end, float percent);

	Vector3 forward;

	Vector3 friction;
	float kFriction;
	Vector3 acceleration;
	float kAcceleration;
	float kSteer;
	float kSteerLerp;
	float maxSpeed;
	LightSource* spotlight;

};

#endif