#ifndef PLAYER_H
#define PLAYER_H

#include "Car.h"
#include "FreeLookCamera.h"
#include "CarUpgrade.h"

enum CAMERA_MODE {
	FIRST_PERSON = 0,
	FIXED_CAR = 1,
};

class Player : public Mesh
{
public:
	Player(const char* meshName, Primitive* primitive, std::string input,
		unsigned int texID = 0, DRAW_MODE drawMode = DRAW_TRIANGLES);
	Player();
	~Player();

	void Update(double dt);
	void setCameraMode(CAMERA_MODE mode);

	Camera* getCamera();
	Camera* getTopdownCamera();
	void setCar(Car* car);
	Car* getCar();

	void switchCameraMode();
	bool isInVehicle;

	CarUpgrade* getUpgrade(std::string car);
	std::vector<CarUpgrade*>* getUpgrades();
	void setMoney(int change);
	int getMoney();

	void getMoneyText(std::string& moneyString, Color& color);


private:

	Car* car;
	std::vector<CarUpgrade*> upgrades;

	std::string input;
	Vector3 racePosition;

	CAMERA_MODE cameraMode;
	FreeLookCamera* firstPerson;
	Camera* topdown;
	Camera* fixedCar;
	float walkSpeed;
	int money;


};

#endif