#include "Player.h"
#include "Application.h"
#include "GL/glew.h"
#include "Manager.h"
#include "Collision.h"

Player::Player(const char* meshName, Primitive* primitive, unsigned int texID, DRAW_MODE drawMode)
	: Mesh(meshName, primitive, texID, true, drawMode)
{
	carOneUnlock = true;
	carTwoUnlock = false;
	carThreeUnlock = false;
	carFourUnlock = false;

	position.Set(-65.8296, 3.0f, 72.4981);
	rotation.Set(0, 0, 0);
	walkSpeed = 4.5f;
	isInVehicle = false;
	cameraMode = FIRST_PERSON;

	firstPerson = new FreeLookCamera(position - Vector3(0.0f, 0.1f, 0.0f) + Vector3(-0.2f, 0.0f, 0.0f));
	fixedCar = new Camera(position + Vector3(0.0f, 8.0f, -6.0f));

	obb->setHalf(Vector3(0.5945f, 1.5505f, 0.378f));
	defaultObb->setHalf(Vector3(0.5945f, 1.5505f, 0.378f));
}

Player::Player()
{

}


Player::~Player()
{
}


void Player::Render(MS& modelStack, MS& viewStack, MS& projectionStack, ShaderProgram* shader)
{
	// Apply Transformations
	//Translate(modelStack, position.x, position.y, position.z);
	//Rotate(modelStack, rotation.x, 1, 0, 0);
	//Rotate(modelStack, rotation.y, 0, 1, 0);
	//Rotate(modelStack, rotation.z, 0, 0, 1);

	
	// Render
	Mesh::Render(modelStack, viewStack, projectionStack, shader);
}

void Player::Update(double dt)
{

	Vector3 right = firstPerson->getRight();
	float rad = Math::DegreeToRadian(firstPerson->getYaw());
	Vector3 forward;


	Vector3 deltaRotation = Vector3(0.0f, -firstPerson->getYaw() + 90, 0.0f);

	Vector3 targetRotation = Utility::Lerp(rotation, deltaRotation, 12.0f * dt);
	if (!isInVehicle && Collision::checkCollisionR(this, deltaRotation, { "ground" }).size() == 0) {
		rotation = targetRotation;
			
	}

	if (!isInVehicle) {

		forward.x = cos(rad);
		forward.z = sin(rad);

		Vector3 translation = Vector3(0, 0, 0);

		// Move Player in the forward direction based on first person camera's rotation
		if (Application::IsKeyPressed('W')) {
			translation += forward * walkSpeed * (float)dt;
		}
		if (Application::IsKeyPressed('A')) {
			translation -=  firstPerson->getRight() * walkSpeed * (float)dt;
		}
		if (Application::IsKeyPressed('S')) {
			translation -= forward * walkSpeed * (float)dt;
		}
		if (Application::IsKeyPressed('D')) {
			translation += firstPerson->getRight() * walkSpeed * (float)dt;
		}

		if (Collision::checkCollisionT(this, translation, { "ground" }).size() == 0)
			position += translation;

	}
	else if (car != nullptr) {
		// Update Position of the Player in the car according to the car's rotation
		float rad = Math::DegreeToRadian(car->rotation.y + 90 + car->currentSteer);
		car->Update(dt);
		position = car->position + Vector3(cos(rad), 0.0f, sin(rad)) * -0.5f + Vector3(0.0f, 1.2f, 0.0f);
		rotation.y = car->rotation.y - car->currentSteer;
	}
	

	// Set Camera's Position
	if (cameraMode == FIRST_PERSON) {
		firstPerson->position = position + Vector3(0.0f, 1.5f, 0.0f) + Vector3(cos(rad), 0.0f, sin(rad)) * 0.3f;
	}
	else if (cameraMode == FIXED_CAR) {
		Vector3 target = position;
		float rad = Math::DegreeToRadian(90 - rotation.y);
		if (isInVehicle) {
			target += Vector3(cos(rad), 0.0f, sin(rad)) * -7.5f + Vector3(0.0f, 3.5f, 0.0f);
			fixedCar->position = Utility::Lerp(fixedCar->position, target, 0.4f);
		}
		else {
			target += Vector3(cos(rad), 0.0f, sin(rad)) * -5.0f + Vector3(0.0f, 1.5f, 0.0f);
			fixedCar->position = Utility::Lerp(fixedCar->position, target, 0.1f);
			
		}
		Vector3 lookAtTarget = position + Vector3(cos(rad), 0.5f, sin(rad)) * 2.5f;
		fixedCar->setTarget(lookAtTarget);
	}

	//topDown.position = position + Vector3(0.0f, 30.0f, 1.0f);
	//topDown.setTarget(position);
	getCamera()->Update(dt);

	if(!isInVehicle)
		Mesh::Update(dt);
}


void Player::setCar(Car* car)
{
	this->car = car;
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

