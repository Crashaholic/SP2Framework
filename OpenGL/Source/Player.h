#ifndef PLAYER_H
#define PLAYER_H

#include "Car.h"
#include "FreeLookCamera.h"

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
	void setCar(Car* car);
	void unlockCar(int carSelected);
	void lockCar(int carSelected);
	void setMoney(int change);
	Car* getCar();
	int getMoney();
	//R
	void getMoneyText(std::string& moneyString, Color& color);
	//R end
	bool getCarsUnlocked(int carID); //for saving player progress

	void switchCameraMode();
	bool isInVehicle;



private:
	std::string input;
	Vector3 racePosition;
	Car* car;
	CAMERA_MODE cameraMode;
	FreeLookCamera* firstPerson;
	Camera* topdown;
	Camera* fixedCar;
	float walkSpeed;
	int money;

	bool carOneUnlock;
	bool carTwoUnlock;
	bool carThreeUnlock;
	bool carFourUnlock;
};

#endif