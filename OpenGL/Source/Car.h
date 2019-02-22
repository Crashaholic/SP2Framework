#ifndef CAR_H
#define CAR_H

#include "Vector3.h"
#include "Mesh.h"
#include "LightSource.h"
#include <fstream>

#include "soloud.h"
#include "soloud_wav.h"

enum PhysicsMode {
	PHYSICS_CAR,
	PHYSICS_PLANE,
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
	PhysicsMode mode;
	void getVelocity(std::string& vel, Color& color);

	void setEngineTier(int newTier);
	void setNitroTier(int newTier);
	void setTireTier(int newTier);
	int getEngineTier();
	int getNitroTier();
	int getTireTier();

	SoLoud::Soloud carEngine;
	SoLoud::Wav carSounds[2];

	enum soundEffects
	{
		SFX_ENTEROREXIT,
		SFX_DRIVING
	};

protected:

	Vector3 calcAcceleration(float accInput, float steerInput, float dt);
	Vector3 calcFriction(float accInput, float steerInput, float dt);

private:

	std::string input;
	float torqueRot;

	float engineAcceleration;
	float reverseAcceleration;
	float maxReverseVelocity;
	float brakingAcceleration;

	bool isOccupied;
	bool isSoundEmitted;

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



	//Tier range: 1 to 3 (1 is the worst, 3 is the best)
	int engineTier; //Increase max velocity
	int nitroTier; //Increase acceleration & unlimit velocity for short duration
	int tireTier; //Allow sharper turns in shorter durations



};

#endif