#ifndef CAR_H
#define CAR_H

#include "Vector3.h"
#include "Mesh.h"
#include "LightSource.h"

#include "soloud.h"
#include "soloud_wav.h"

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
	Vector3 updatePosition(float accInput, float steerInput, float dt);
	Vector3 forward;

private:

	float engineAcceleration;
	float reverseAcceleration;
	float maxReverseVelocity;
	float brakingAcceleration;


	bool start;

	bool isOccupied;
	bool isSoundEmitted;

	float steerAmount;

	float kBraking;
	float kMass;
	float kDrag;
	float kFriction;
	float steerAngle;

	//Tier range: 1 to 3 (1 is the worst, 3 is the best)
	int engineTier; //Increase max velocity
	int nitroTier; //Increase acceleration & unlimit velocity for short duration
	int tireTier; //Allow sharper turns in shorter durations


	float previousInputs[2];

};

#endif