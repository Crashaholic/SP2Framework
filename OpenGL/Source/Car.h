#ifndef CAR_H
#define CAR_H

#include "Vector3.h"
#include "Mesh.h"
#include "LightSource.h"

class Car : public Mesh
{
public:
	Car(const char* meshName, Primitive* primitive, unsigned int texID = 0, DRAW_MODE drawMode = DRAW_TRIANGLES);
	~Car();

	void Update(double dt);
	void Render(MS& modelStack, MS& viewStack, MS& projectionStack, ShaderProgram* shader);

	void setOccupied(bool isOccupied);
	float currentSteer;
	float angularVelocity;

private:
	Car();
	Vector3 calcResultant(float accInput, float steerInput, float dt);
	void switchGears(float rpm);
	float getTurnRatio(float gear);
	float getFinalTurnRatio(float gear);

	float engineAcceleration;
	float reverseAcceleration;
	float maxReverseVelocity;
	float brakingAcceleration;
	Vector3 forward;


	bool isOccupied;

	float steerAmount;

	float kBraking;
	float kMass;
	float kDrag;
	float kFriction;	

	float steerAngle;

	int currentGear;

};

#endif