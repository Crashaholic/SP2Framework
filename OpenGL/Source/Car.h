#ifndef CAR_H
#define CAR_H

#include "Vector3.h"
#include "Mesh.h"
#include "LightSource.h"
#include <fstream>
#include "soloud.h"
#include "soloud_wav.h"


enum soundEffects
{
	SFX_ENTEROREXIT,
	SFX_DRIVING
};

class Car : public Mesh
{
public:
	Car(const char* meshName, Primitive* primitive, std::string input, unsigned int texID = 0, DRAW_MODE drawMode = DRAW_TRIANGLES);
	Car();
	~Car();

	virtual void Update(double dt);
	void setOccupied(bool isOccupied);
	float currentSteer;
	float angularVelocity;
	Vector3 forward;
	void getVelocity(std::string& vel, Color& color);
	int getWaypointID();
	int getLaps();
	double getTiming();
	bool hasFinished();

	SoLoud::Soloud carEngine;
	SoLoud::Wav carSounds[2];

protected:

	Vector3 calcAcceleration(float accInput, float steerInput, float dt);
	Vector3 calcFriction(float accInput, float steerInput, float dt);

private:

	void updateWaypoint();
	bool finished;
	int laps;
	double timer;
	std::string input;
	float torqueRot;
	int waypointID;

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