#include "Player.h"
#include "Application.h"
#include "GL/glew.h"
#include "Manager.h"
#include "Collision.h"

Player::Player(const char* meshName, Primitive* primitive, std::string input, unsigned int texID, DRAW_MODE drawMode)
	: Mesh(meshName, primitive, texID, true, true, "player", drawMode) {


	this->input = input;

	walkSpeed = 3.0f;
	isInVehicle = false;
	cameraMode = FIRST_PERSON;
	money = 0;

	firstPerson = new FreeLookCamera(position - Vector3(0.0f, 0.1f, 0.0f) + Vector3(-0.2f, 0.0f, 0.0f));
	fixedCar = new Camera(position + Vector3(0.0f, 8.0f, -6.0f));
	topdown = new Camera(position + Vector3(0.0f, 8.0f, -6.0f));
	
	carOneUnlock = true;
	carTwoUnlock = false;
	carThreeUnlock = false;
	carFourUnlock = false;

}

Player::Player()
{

}


Player::~Player()
{
}

void Player::Update(double dt) {

	Vector3 right = firstPerson->getRight();
	float rad = Math::DegreeToRadian(firstPerson->getYaw());
	Vector3 forward;
	forward.x = cos(rad);
	forward.z = sin(rad);
	forward.Normalize();



	if (!isInVehicle) {

		Vector3 translation = Vector3(0, 0, 0);

		// Input 0 - W / I
		// Input 1 - A / J
		// Input 2 - S / K
		// Input 3 - D / L


		// Move Player in the forward direction based on first person camera's rotation
		if (Application::IsKeyPressed(input[0])) {
			translation += forward * walkSpeed * (float)dt;
		}
		if (Application::IsKeyPressed(input[1])) {
			translation -=  firstPerson->getRight() * walkSpeed * (float)dt;
		}
		if (Application::IsKeyPressed(input[2])) {
			translation -= forward * walkSpeed * (float)dt;
		}
		if (Application::IsKeyPressed(input[3])) {
			translation += firstPerson->getRight() * walkSpeed * (float)dt;
		}



		// Only translate if there is no collision
		std::vector<Mesh*> collided = Collision::checkCollisionT(this, translation, { "ground", "pad1" });
		if (translation != Vector3(0,0,0) && collided.size() == 0) {
			position += translation;
		}


		Vector3 deltaRotation = Vector3(0.0f, -firstPerson->getYaw() + 90, 0.0f);

		Vector3 targetRotation = Utility::Lerp(rotation, deltaRotation, 12.0f * dt);

		// Rotate only if there is no collision
		if (!isInVehicle && Collision::checkCollisionR(this, deltaRotation, { "ground", "pad1" }).size() == 0) {
			rotation = targetRotation;
		}

	}
	else if (car != nullptr) {
		// Update Position of the Player in the car according to the car's rotation
		float rad = Math::DegreeToRadian(90 + car->currentSteer);
		car->Update(dt);
		position = car->position + Vector3(cos(rad), 0, sin(rad)) * -0.5f + Vector3(0.0f, 1.2f, 0.0f);
		rotation = car->rotation;
	}


	// Set Camera's Position
	if (cameraMode == FIRST_PERSON) {
		firstPerson->position = position + Vector3(0.0f, 1.2f, 0.0f) + forward * 0.2f;
	}
	else if (cameraMode == FIXED_CAR) {
		// Target = Camera's position
		// Look At Target = Where the Camera is looking at
		Vector3 target = position;
		Vector3 lookAtTarget = position;

		float rad = Math::DegreeToRadian(90 - rotation.y);
		Vector3 anotherForward;
		anotherForward.x = cos(rad);
		anotherForward.z = sin(rad);

		if (isInVehicle) {
			target += anotherForward * -7.5f + Vector3(0.0f, 2.5f, 0.0f);
			lookAtTarget += anotherForward * 2.5f + Vector3(0.0f, 1.5f, 0.0);
		}
		else {
			target += anotherForward * -5.0f + Vector3(0.0f, 3.5f, 0.0f);
			lookAtTarget = position + anotherForward * 2.5f + Vector3(0, 0.5f, 0.0);
		}
		fixedCar->position = Utility::Lerp(fixedCar->position, target, 0.9f);
		fixedCar->setTarget(lookAtTarget);
	}

	topdown->position = position + Vector3(0.1f, 30.0f, 0.0f);
	topdown->setTarget(position);

	if (!isInVehicle)
		Mesh::Update(dt);

	getCamera()->Update(dt);

	Manager::getInstance()->getLevel()->getTree()->Update(this);

}


void Player::setCar(Car* car)
{
	this->car = car;
	isInVehicle = true;
	car->setOccupied(true);
	collisionEnabled = false;
	setCameraMode(FIXED_CAR);
}

void Player::setMoney(int change)
{
	this->money += change;
}

void Player::unlockCar(int carSelected)
{
	if (carSelected == 2)
	{
		carTwoUnlock = true;
	}
	if (carSelected == 3)
	{
		carThreeUnlock = true;
	}
	if (carSelected == 4)
	{
		carFourUnlock = true;
	}
}

void Player::lockCar(int carSelected)
{
	if (carSelected == 2)
	{
		carTwoUnlock = false;
	}
	if (carSelected == 3)
	{
		carThreeUnlock = false;
	}
	if (carSelected == 4)
	{
		carFourUnlock = false;
	}
}

Car* Player::getCar()
{
	return car;
}

int Player::getMoney()
{
	return money;
}

void Player::getMoneyText(std::string& moneyString, Color& color)
{
	moneyString = std::to_string(money);
	color.Set(1, 1, 0);

}

bool Player::getCarsUnlocked(int carID)
{
	if (carID == 1)
		return carOneUnlock;
	if (carID == 2)
		return carTwoUnlock;
	if (carID == 3)
		return carThreeUnlock;
	if (carID == 4)
		return carFourUnlock;
}

Camera* Player::getCamera()
{
	if (cameraMode == FIRST_PERSON)
		return firstPerson;
	else if (cameraMode == FIXED_CAR)
		return fixedCar;
	return nullptr;
}

void Player::switchCameraMode()
{
	if (cameraMode == FIRST_PERSON)
		cameraMode = FIXED_CAR;
	else
		cameraMode = FIRST_PERSON;
}

void Player::setCameraMode(CAMERA_MODE mode)
{

	cameraMode = mode;
}

