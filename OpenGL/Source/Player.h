#ifndef PLAYER_H
#define PLAYER_H

#include "Car.h"
#include "FreeLookCamera.h"

enum CAMERA_MODE {
	FIRST_PERSON = 0,
	THIRD_PERSON = 1,
};

class Player : public Mesh
{
public:
	Player(const char* meshName, Primitive* primitive, unsigned int texID = 0, DRAW_MODE drawMode = DRAW_TRIANGLES);
	Player();
	~Player();

	void Update(double dt);
	void Render(MS& modelStack, MS& viewStack, MS& projectionStack, ShaderProgram* shader);
	void setCameraMode(CAMERA_MODE mode);


	FreeLookCamera* getCamera();
	void setCar(Car* car);
	Car* getCar();

	void switchCameraMode();
	bool isInVehicle;


private:
	Car* car;
	CAMERA_MODE cameraMode;
	FreeLookCamera* camera;
	float walkSpeed;
};

#endif