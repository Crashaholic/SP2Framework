#include "Car.h"
#include "Application.h"
#include "GUIManager.h"
#include "Utility.h"
#include "Manager.h"

Car::Car(const char* meshName, Primitive* primitive, unsigned int texID, DRAW_MODE drawMode)
	: Mesh(meshName, primitive, texID, true, drawMode) {



	position.Set(0.0f, 100.0f, 5.0f);
	rotation.Set(0, 0, 0);
	velocity.Set(0, 0, 0);
	forward.Set(0, 0, 1);

	kBraking = 2.0f;
	kMass = 50.0f;
	kDrag = 0.1f;
	kFriction = 0.20f;
	maxReverseVelocity = -0.5f;

	engineAcceleration = brakingAcceleration = reverseAcceleration = 0.0f;
	isOccupied = false;
	currentSteer = steerAmount = 0.0f;

	obb->setHalf(Vector3(2.192, 1.2445, 4.289));
	defaultObb->setHalf(Vector3(2.192, 1.2445, 4.289));
}


Car::~Car()
{
}

void Car::setOccupied(bool isOccupied) {
	this->isOccupied = isOccupied;
}


void Car::Update(double dt)
{

	if (isOccupied) {

		float accInput = 0.0f;
		float steerInput = 0.0f;

		if (Application::IsKeyPressed('W'))
			accInput = 1.0f;
		else if (Application::IsKeyPressed('S'))
			accInput = -1.0f;
		else
			accInput = 0.0f;

		if (Application::IsKeyPressed('A'))
			steerInput = -1;
		else if (Application::IsKeyPressed('D'))
			steerInput = 1;
		else
			steerInput = 0;

		calcResultant(accInput, steerInput, dt);
		
	}

	Mesh::Update(dt);
}

Vector3 Car::calcResultant(float accInput, float steerInput, float dt) {

	Vector3 resultant, traction, rotDrag, drag, friction, braking, acceleration, lateral, reverse, engineForward = Vector3(0,0,0);



	// Steering Car
	if (velocity.Length() > 0.01f)
		steerAngle = steerInput * 25;
	else if (accInput != -1)
		steerAngle = 0;

	if (accInput < 0.0f)
		steerAngle = -steerAngle;

	currentSteer = Utility::Lerp(currentSteer, currentSteer + steerAngle, (float)dt * 0.75f);

	float angle = rotation.y + 90.0f + currentSteer;
	float rad = Math::DegreeToRadian(angle);
	forward.x = cos(rad);
	forward.z = sin(rad);
	forward.Normalized();

	float velocityDir = Utility::Sign(velocity);

	if (accInput == 1) {
		engineAcceleration = Utility::Lerp(engineAcceleration, accInput, 5.0f * dt);
		reverseAcceleration = 0.0f;
	}
	else if (accInput == -1 && velocityDir == 1.0f) {
		brakingAcceleration = Utility::Lerp(brakingAcceleration, accInput, 0.50f * dt);
	}
	else {
		engineAcceleration = 0.0f;
	}

	


	reverseAcceleration = Utility::Lerp(reverseAcceleration, accInput, 2.5f * dt);
	reverse = reverseAcceleration * forward * 10.0 * dt;

	if (accInput != -1) {
		brakingAcceleration = 0.0f;
	}


	if (velocity.Length() < 0.001f)
		velocity.SetZero();

	if (accInput == 1) {
		engineForward = forward * engineAcceleration * 20.0f * dt;
	}
	else if (accInput == -1) {
		if (velocityDir == 1 && steerInput == 0)
		{
			if(velocity.Length() > 0.0f)
				braking = velocityDir * velocity.Normalized() * brakingAcceleration * 18.0f * dt;
		
		}
		
	}
	reverse = forward * reverseAcceleration * 15.0f * dt;
	if (accInput != -1 && (steerInput == 0 || reverse.Length() < 1.0f))
		reverse.SetZero();

	if (velocity.Length() != 0.0f) {
		friction = (kFriction * -velocity.Normalized());
		if(accInput != -1)
			drag = kDrag * velocity.Length() * -velocity.Normalized();

		if (steerInput != 0)
			drag *= 1.2f;

	}


	acceleration = friction + engineForward + braking + drag + reverse;
	if (fabs(steerAmount) > 0.05f) {
		acceleration *= 0.7f;
	}

	

	std::cout << "Reverse: " << reverse  << std::endl;
	

	velocity += acceleration * dt;
	
	
	//velocity += angularVelocity;
	//std::cout << "Braking: " << braking << std::endl;
	//std::cout << "Circle Radius: " << circleRadius << std::endl;

	return resultant;
}

void Car::switchGears(float rpm) {


	
}

float Car::getTurnRatio(float gear) {
	if (gear == 1) {
		return 3.13f;
	}
	else if (gear == 2) {
		return 2.59f;
	}
	else if (gear == 3) {
		return 1.96f;
	}
	else if (gear == 4) {
		return 1.24f;
	}
	else if (gear == 5 || gear == 6) {
		return 0.98f;
	}
	else if (gear == 7) {
		return 0.84f;
	}
}

float Car::getFinalTurnRatio(float gear) {
	if (gear == 1 || gear == 4 || gear == 5) {
		return 4.89f;
	}
	else if (gear == 2 || gear == 3 || gear == 6 || gear == 7) {
		return 3.94f;
	}

}

void Car::Render(MS& modelStack, MS& viewStack, MS& projectionStack, ShaderProgram* shader)
{


	//// Draw Velocity
	//int vSpeed = (int)(velocity.Length() * 161);
	//GUIManager::getInstance()->renderText("game", 10, 590, "V: " + std::to_string(vSpeed) + " mph", 0.35f, Color(1, 0, 0), TEXT_ALIGN_BOTTOMLEFT);


	// Render
	Mesh::Render(modelStack, viewStack, projectionStack, shader);




	
}
