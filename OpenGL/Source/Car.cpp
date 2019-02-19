#include "Car.h"
#include "Application.h"
#include "GUIManager.h"
#include "Utility.h"
#include "Manager.h"


Car::Car(const char* meshName, Primitive* primitive, unsigned int texID, DRAW_MODE drawMode)
	: Mesh(meshName, primitive, texID, true, drawMode)
{

	carEngine.init();
	carSounds[SFX_ENTEROREXIT].load("Music//SFX_CarEnter.wav");
	carSounds[SFX_DRIVING].load("Music//SFX_CarDriving.wav");

	position.Set(0.0f, 100.0f, 5.0f);
	rotation.Set(0, 0, 0);
	velocity.Set(0, 0, 0);
	forward.Set(0, 0, 1);

	kBraking = 2.0f;
	kMass = 50.0f;
	kDrag = 0.2f;
	kFriction = 0.22f;
	maxReverseVelocity = -0.5f;

	engineAcceleration = brakingAcceleration = reverseAcceleration = 0.0f;
	isOccupied = false;
	currentSteer = steerAmount = 0.0f;

	start = false;
	isSoundEmitted = false;

	engineTier = 1;
	nitroTier = 1;
	tireTier = 1;

	previousInputs[0] = previousInputs[1] = 0;

	obb->setHalf(Vector3(2.192, 1.2445, 4.289));
	defaultObb->setHalf(Vector3(2.192, 1.2445, 4.289));
}

Car::Car()
{
	carEngine.init();
	carSounds[SFX_ENTEROREXIT].load("Music//SFX_CarEnter.wav");
	carSounds[SFX_DRIVING].load("Music//SFX_CarDriving.wav");
}

Car::~Car()
{
	carEngine.deinit();
}

void Car::setOccupied(bool isOccupied)
{
	this->isOccupied = isOccupied;
	carEngine.play(carSounds[SFX_ENTEROREXIT]);
}

void Car::setEngineTier(int newTier)
{
	this->engineTier = newTier;
}

void Car::setNitroTier(int newTier)
{
	this->nitroTier = newTier;
}

void Car::setTireTier(int newTier)
{
	this->engineTier = newTier;
}


void Car::Update(double dt)
{

	if (isOccupied)
	{

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

		if (Application::IsKeyPressed('K') && !start) start = true;

		if ((velocity.x > 0.15 || velocity.z > 0.15 || velocity.x < -0.15 || velocity.z < -0.15) && isSoundEmitted == false)
		{
			isSoundEmitted = true;
			carSounds[SFX_DRIVING].setLooping(1);
			carEngine.play(carSounds[SFX_DRIVING], 0.5f, 1);
			carEngine.seek(carEngine.play(carSounds[SFX_DRIVING], 0.5f, 1), 1.0f);
			carEngine.setPause(carEngine.play(carSounds[SFX_DRIVING]), 0);

		}
		else if (velocity.x == 0 && velocity.z == 0)
		{
			carSounds[SFX_DRIVING].stop();
			isSoundEmitted = false;
		}

		velocity += updatePosition(accInput, steerInput, dt);


	}

	Mesh::Update(dt);
}

Vector3 Car::updatePosition(float accInput, float steerInput, float dt)
{

	Vector3 resultant, rotDrag, drag, friction, braking, acceleration, reverse, engineForward = Vector3(0, 0, 0);



	// Steering Car
	if (velocity.Length() > 0.01f)
		steerAngle = steerInput * 30;
	else if (accInput != -1)
		steerAngle = 0;

	if (accInput < 0.0f)
		steerAngle = -steerAngle;

	currentSteer = Utility::Lerp(currentSteer, currentSteer + steerAngle, (float)dt * 0.70f);

	float angle = rotation.y + 90.0f + currentSteer;
	float rad = Math::DegreeToRadian(angle);
	forward.x = cos(rad);
	forward.z = sin(rad);
	forward.Normalized();

	float velocityDir = Utility::Sign(velocity);

	if (accInput == 1)
	{
		engineAcceleration = Utility::Lerp(engineAcceleration, accInput, 5.0f * dt);
		reverseAcceleration = 0.0f;
	}
	else if (accInput == -1 && velocityDir == 1.0f)
	{
		brakingAcceleration = Utility::Lerp(brakingAcceleration, accInput, 0.50f * dt);
	}
	else
	{
		engineAcceleration = 0.0f;
	}




	reverseAcceleration = Utility::Lerp(reverseAcceleration, accInput, 2.5f * dt);
	reverse = reverseAcceleration * forward * 10.0 * dt;

	if (accInput != -1)
	{
		brakingAcceleration = 0.0f;
	}


	if (velocity.Length() < 0.001f)
		velocity.SetZero();

	if (accInput == 1)
	{
		engineForward = forward * engineAcceleration * 20.0f * dt;
	}
	else if (accInput == -1)
	{
		if (velocityDir == 1 && steerInput == 0)
		{
			if (velocity.Length() > 0.0f)
				braking = velocityDir * velocity.Normalized() * brakingAcceleration * 18.0f * dt;

		}

	}
	reverse = forward * reverseAcceleration * 15.0f * dt;
	if (accInput != -1 && (steerInput == 0 || reverse.Length() < 1.0f))
		reverse.SetZero();

	if (velocity.Length() != 0.0f)
	{
		friction = (kFriction * -velocity.Normalized());
		if (accInput != -1)
			drag = kDrag * velocity.Length() * -velocity.Normalized();

		if (steerInput != 0)
			drag *= 1.2f;

	}


	acceleration = friction + engineForward + braking + drag + reverse;
	if (fabs(steerAmount) > 0.05f)
	{
		acceleration *= 0.7f;
	}


	if (accInput != previousInputs[0] && start)
	{
		std::cout << "Acc: " << position << ", Rot: " << rotation.y - currentSteer << "-> A: " << accInput << std::endl;
	}
	else if (steerInput != previousInputs[1] && start)
	{
		std::cout << "Steering: " << position << ", Rot: " << rotation.y - currentSteer << " -> S: " << steerInput << std::endl;
	}



	previousInputs[0] = accInput;
	previousInputs[1] = steerInput;
	return acceleration * dt;
}



void Car::Render(MS& modelStack, MS& viewStack, MS& projectionStack, ShaderProgram* shader)
{


	//// Draw Velocity
	//int vSpeed = (int)(velocity.Length() * 161);
	//GUIManager::getInstance()->point., 10, 590, "V: " + std::to_string(vSpeed) + " mph", 0.35f, Color(1, 0, 0), TEXT_ALIGN_BOTTOMLEFT);


	// Render
	Mesh::Render(modelStack, viewStack, projectionStack, shader);





}

