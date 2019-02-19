#include "Player.h"
#include "Application.h"
#include "GL/glew.h"
#include "Manager.h"
#include "Collision.h"

Player::Player(const char* meshName, Primitive* primitive, unsigned int texID, DRAW_MODE drawMode)
	: Mesh(meshName, primitive, texID, true, true, "player", drawMode) {


	walkSpeed = 3.0f;
	isInVehicle = false;
	cameraMode = FIRST_PERSON;

	firstPerson = new FreeLookCamera(position - Vector3(0.0f, 0.1f, 0.0f) + Vector3(-0.2f, 0.0f, 0.0f));
	fixedCar = new Camera(position + Vector3(0.0f, 8.0f, -6.0f));
	topdown = new Camera(position + Vector3(0.0f, 8.0f, -6.0f));
	//obb->setHalf(Vector3(0.5945f, 1.5505f, 0.378f));
	//defaultObb->setHalf(Vector3(0.5945f, 1.5505f, 0.378f));
	
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

void Player::Update(double dt) {

	Vector3 right = firstPerson->getRight();
	float rad = Math::DegreeToRadian(firstPerson->getYaw());
	Vector3 forward;
	forward.x = cos(rad);
	forward.z = sin(rad);
	forward.Normalize();

	Vector3 deltaRotation = Vector3(0.0f, -firstPerson->getYaw() + 90, 0.0f);

	Vector3 targetRotation = Utility::Lerp(rotation, deltaRotation, 12.0f * dt);
	
	// Rotate only if there is no collision
	if (!isInVehicle && Collision::checkCollisionR(this, deltaRotation, { "ground", "pad1" }).size() == 0) {
		rotation = targetRotation;
	}

	if (!isInVehicle) {

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

		// Only translate if there is no collision
		std::vector<Mesh*> collided = Collision::checkCollisionT(this, translation, { "ground", "pad1" });
		if (translation != Vector3(0,0,0) && collided.size() == 0) {
			position += translation;
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
			target += anotherForward * -7.5f + Vector3(0.0f, 3.5f, 0.0f);
			lookAtTarget += anotherForward * 2.5f;
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


}


void Player::setCar(Car* car) {
	this->car = car;
}

Car* Player::getCar() {
	return car;
}

Camera* Player::getTopdownCamera() {
	return topdown;
}

Camera* Player::getCamera() {
	if (cameraMode == FIRST_PERSON)
		return firstPerson;
	else if (cameraMode == FIXED_CAR)
		return fixedCar;
	return nullptr;
}

void Player::switchCameraMode() {
	if (cameraMode == FIRST_PERSON)
		cameraMode = FIXED_CAR;
	else
		cameraMode = FIRST_PERSON;
}

void Player::setCameraMode(CAMERA_MODE mode)
{
	cameraMode = mode;
}

