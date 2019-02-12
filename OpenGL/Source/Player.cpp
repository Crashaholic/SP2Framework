#include "Player.h"
#include "Application.h"
#include "GL/glew.h"
#include "Manager.h"

Player::Player(const char* meshName, Primitive* primitive, unsigned int texID, DRAW_MODE drawMode)
	: Mesh(meshName, primitive, texID, true, drawMode) {

	position.Set(0, 100.0f, 0);
	rotation.Set(0, 0, 0);
	walkSpeed = 4.5f;
	isInVehicle = false;
	cameraMode = FIRST_PERSON;

	camera = new FreeLookCamera(position - Vector3(0.0f, 0.1f, 0.0f) + Vector3(-0.2f, 0.0f, 0.0f));


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

		// Move Player in the forward direction based on first person camera's rotation
		if (Application::IsKeyPressed('W')) {
			position += forward * walkSpeed * (float)dt;
		}
		if (Application::IsKeyPressed('A')) {
			position -= camera->getRight() * walkSpeed * (float)dt;
		}
		if (Application::IsKeyPressed('S')) {
			position -= forward * walkSpeed * (float)dt;
		}
		if (Application::IsKeyPressed('D')) {
			position += camera->getRight() * walkSpeed * (float)dt;
		}

	}
	else if (car != nullptr) {
		// Update Position of the Player in the car according to the car's rotation
		//float rad = Math::DegreeToRadian(car->rotation.y + 90 + car->currentSteer);
		//car->Update(dt);
		//position = car->position + Vector3(cos(rad), 0.0f, sin(rad)) * -0.5f + Vector3(0.0f, 3.0f, 0.0f);
		//rotation.y = car->rotation.y - car->currentSteer;
	}
	

	// Set Camera's Position
	if (cameraMode == FIRST_PERSON) {
		camera->position = position + Vector3(cos(rad), 0.0f, sin(rad)) * 0.3f;
		//rotation.y = -firstPerson->getYaw() + 90;
	}
	else if (cameraMode == THIRD_PERSON) {
		Vector3 target = position + Vector3(0.0f, 1.0f, 0.0f) + Vector3(cos(rad), 0.0f, sin(rad)) * -5.0f;
		//camera->position = Utility::sLerp(camera->position, target, 0.1f);
		camera->position = Utility::Lerp(camera->position, target, 18.0f * dt);
	}

	//topDown.position = position + Vector3(0.0f, 30.0f, 1.0f);
	//topDown.setTarget(position);
	camera->Update(dt);
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

