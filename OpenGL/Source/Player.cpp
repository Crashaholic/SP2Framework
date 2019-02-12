#include "Player.h"
#include "Application.h"
#include "GL/glew.h"
#include "Manager.h"
#include "Collision.h"

Player::Player(const char* meshName, Primitive* primitive, unsigned int texID, DRAW_MODE drawMode)
	: Mesh(meshName, primitive, texID, true, drawMode) {

	position.Set(0, 5.0f, 0);
	rotation.Set(0, 0, 0);
	walkSpeed = 4.5f;
	isInVehicle = false;
	cameraMode = FIRST_PERSON;

	camera = new FreeLookCamera(position - Vector3(0.0f, 0.1f, 0.0f) + Vector3(-0.2f, 0.0f, 0.0f));

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

void Player::Update(double dt) {

	Vector3 right = camera->getRight();
	float rad = Math::DegreeToRadian(camera->getYaw());
	Vector3 forward;

	rotation = Utility::Lerp(rotation, Vector3(0.0f, -camera->getYaw() + 90, 0.0f), 12.0f * dt);

	if (!isInVehicle) {

		forward.x = cos(rad);
		forward.z = sin(rad);
		forward.Normalize();

		Vector3 translation = Vector3(0, 0, 0);

		// Move Player in the forward direction based on first person camera's rotation
		if (Application::IsKeyPressed('W')) {
			translation = forward * walkSpeed * (float)dt;
		}
		if (Application::IsKeyPressed('A')) {
			translation =  -camera->getRight() * walkSpeed * (float)dt;
		}
		if (Application::IsKeyPressed('S')) {
			translation = -forward * walkSpeed * (float)dt;
		}
		if (Application::IsKeyPressed('D')) {
			translation = camera->getRight() * walkSpeed * (float)dt;
		}

		if (Collision::checkCollision(this, translation, { "ground" }).size() == 0)
			position += translation;

	}
	else if (car != nullptr) {
		// Update Position of the Player in the car according to the car's rotation
		float rad = Math::DegreeToRadian(car->rotation.y + 90 + car->currentSteer);
		car->Update(dt);
		position = car->position + Vector3(cos(rad), 0.0f, sin(rad)) * -0.5f + Vector3(0.0f, 1.2f, 0.0f);
		rotation.y = car->rotation.y;
	}
	

	// Set Camera's Position
	if (cameraMode == FIRST_PERSON) {
		camera->position = position + Vector3(0.0f, 1.5f, 0.0f) + Vector3(cos(rad), 0.0f, sin(rad)) * 0.3f;
		//rotation.y = -firstPerson->getYaw() + 90;
	}
	else if (cameraMode == THIRD_PERSON) {
		Vector3 target = position + Vector3(cos(rad), 0.0f, sin(rad)) * -5.0f;
		if (isInVehicle) {
			target += Vector3(0.0f, 3.0f, 0.0f);
		}
		else {
			target += Vector3(0.0f, 1.5f, 0.0f);
		}
		//camera->position = Utility::sLerp(camera->position, target, 0.1f);
		camera->position = Utility::Lerp(camera->position, target, 18.0f * dt);
	}

	//topDown.position = position + Vector3(0.0f, 30.0f, 1.0f);
	//topDown.setTarget(position);
	camera->Update(dt);

	if(!isInVehicle)
		Mesh::Update(dt);
}


void Player::setCar(Car* car) {
	this->car = car;
}

Car* Player::getCar() {
	return car;
}



FreeLookCamera* Player::getCamera() {
	return camera;
}

void Player::switchCameraMode() {
	if (cameraMode == FIRST_PERSON)
		cameraMode = THIRD_PERSON;
	else
		cameraMode = FIRST_PERSON;
}

void Player::setCameraMode(CAMERA_MODE mode)
{
	cameraMode = mode;
}

