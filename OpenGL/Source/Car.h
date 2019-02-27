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
	Car(const char* meshName, Primitive* primitive, std::string input, float nitro, unsigned int texID = 0, DRAW_MODE drawMode = DRAW_TRIANGLES);
	Car();
	~Car();

	virtual void Update(double dt);
	void setOccupied(std::string name, bool isOccupied);
	float currentSteer;
	Vector3 forward;
	void getVelocity(std::string& vel, Color& color);
	int getWaypointID();
	void setWaypointID(int n);
	int getLaps();
	double getTiming();
	bool hasFinished();

	std::string getNitro();
	std::string getThruster();	




protected:

	Vector3 calcAcceleration(float accInput, float steerInput, float dt);
	Vector3 calcFriction(float accInput, float steerInput, float dt);

private:

	SoLoud::Soloud carEngine;
	SoLoud::Wav carSounds[2];
	
	bool isSoundPlaying;

	void setModel(Primitive* primitive);
	float shakeDuration;
	Vector3 shakeAmount;

	std::string playerName;
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
	float nitro;

	int nitroTier;
	int engineTier;
	int tyreTier;

	// [DEBUG ONLY]
	bool start;
	std::ofstream handle;
	float previousInputs[2];




};

#endif