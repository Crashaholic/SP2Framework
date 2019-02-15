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

	virtual void Update(double dt);
	void Render(MS& modelStack, MS& viewStack, MS& projectionStack, ShaderProgram* shader);

	void setOccupied(bool isOccupied);
	float currentSteer;
	float angularVelocity;

protected:
	Vector3 updatePosition(float accInput, float steerInput, float dt);
	Vector3 forward;

private:

	float engineAcceleration;
	float reverseAcceleration;
	float maxReverseVelocity;
	float brakingAcceleration;
	

	bool start;

	bool isOccupied;

	float steerAmount;

	float kBraking;
	float kMass;
	float kDrag;
	float kFriction;	
	float steerAngle;


	float previousInputs[2];

};

#endif