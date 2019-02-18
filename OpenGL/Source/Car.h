#ifndef CAR_H
#define CAR_H

#include "Vector3.h"
#include "Mesh.h"
#include "LightSource.h"
#include <fstream>

enum PhysicsMode {
	PHYSICS_CAR,
	PHYSICS_PLANE,
};

class Car : public Mesh
{
public:
	Car(const char* meshName, Primitive* primitive, unsigned int texID = 0, DRAW_MODE drawMode = DRAW_TRIANGLES);
	Car();
	~Car();

	virtual void Update(double dt);
	void setOccupied(bool isOccupied);
	float currentSteer;
	float angularVelocity;
	Vector3 forward;
	PhysicsMode mode;
	float xDelta;
	float zDelta;
protected:
	Vector3 updatePosition(float accInput, float steerInput, float dt);


private:

	float engineAcceleration;
	float reverseAcceleration;
	float maxReverseVelocity;
	float brakingAcceleration;

	bool isOccupied;
	float steerAmount;
	float kBraking;
	float kMass;
	float kDrag;
	float kFriction;
	float steerAngle;
	float thrust;
	float thrusters = 300.0f;

	// [DEBUG ONLY]
	bool start;
	std::ofstream handle;
	float previousInputs[2];





};

#endif