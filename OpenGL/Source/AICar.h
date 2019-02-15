#ifndef AICAR_H
#define AICAR_H

#include "Car.h"
#include "AIWaypoint.h"

class AICar : public Car
{
public:
	AICar(const char* meshName, Primitive* primitive, unsigned int texID = 0, DRAW_MODE drawMode = DRAW_TRIANGLES);
	AICar();
	~AICar();
	void Update(double dt);
	void loadWaypoints();
private:
	void getInputs();
	float currentAccInput;
	float currentSteerInput;
	int currentID;
	std::vector<AIWaypoint*> waypoints;
};

#endif

