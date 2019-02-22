#include "Car.h"
#include "Application.h"
#include "GUIManager.h"
#include "Utility.h"
#include "Collision.h"
#include "Manager.h"
#include "Player.h"
#include "AICar.h"


Car::Car(const char* meshName, Primitive* primitive, std::string input, unsigned int texID, DRAW_MODE drawMode)
	: Mesh(meshName, primitive, texID, true, true, "car", drawMode)
{


	this->input = input;

	velocity.Set(0, 0, 0);
	forward.Set(0, 0, 1);

	kBraking = 2.0f;
	kMass = 50.0f;
	kDrag = 0.2f;
	kFriction = 0.22f;
	maxReverseVelocity = -0.5f;

	engineAcceleration = brakingAcceleration = reverseAcceleration = 0.0f;
	isOccupied = false;
	steerAmount = 0.0f;
	currentSteer = -90.0f;

	start = false;

	previousInputs[0] = previousInputs[1] = 0;

	mode = PHYSICS_CAR;
	thrusters = 300.0f;
	thrust = 0.0f;
	torqueRot = 0.0f;
}

Car::Car()
{

}

Car::~Car()
{

}

void Car::setOccupied(bool isOccupied)
{
	this->isOccupied = isOccupied;
}


void Car::Update(double dt)
{

	onGroundCheck(dt);



	float accInput = 0.0f;
	float steerInput = 0.0f;

	if (isOccupied)
	{



		// Input 0 - W / I
		// Input 1 - A / J
		// Input 2 - S / K
		// Input 3 - D / L

		if (Application::IsKeyPressed(input[0]))
			accInput = 1.0f;
		else if (Application::IsKeyPressed(input[2]))
			accInput = -1.0f;
		else
			accInput = 0.0f;

		if (Application::IsKeyPressed(input[1]))
			steerInput = -1;
		else if (Application::IsKeyPressed(input[3]))
			steerInput = 1;
		else
			steerInput = 0;

		//if (Application::IsKeyPressed('K') && !start) start = true;

		float thrustInput = 0.0f;

		// Modify
		if (Application::IsKeyPressed(input[4]) /*&& thrusters > 0.0f*/) {
			std::cout << thrusters << std::endl;
			thrustInput = 1.0f;
			thrust = Utility::Lerp(thrust, 1.0f, 8.0f * dt);
			thrusters -= thrust * dt * 20.0;
			velocity.y += thrust * dt;
			
		}
		else {
			thrust = 0;
			thrustInput = 0.0f;
		}



		
		velocity += calcAcceleration(accInput, steerInput, dt);
	}

	velocity += calcFriction(accInput, steerInput, dt);

	//if (name == "car")
	//	std::cout << "R: " << velocity << std::endl;


	std::vector<Mesh*> collided = Collision::checkCollisionTypes(this, velocity, { "car", "ai"});

	if (velocity != Vector3(0, 0, 0) && collided.size() != 0) {
		for (int i = 0; i < collided.size(); i++) {
			Car* car = dynamic_cast<Car*>(collided[i]);
			if (car == nullptr) continue;
			//std::cout << "AI's velocity: " << ai->velocity.Length() << std::endl;
			//std::cout << "Car's velocity: " << velocity.Length() << std::endl;

			// Find difference in their forward 
		

			float kForwardDiff = 1.0f - abs(car->forward.Dot(-forward));
			float finalVelCar = 0.0f;
			float finalVelCar2 = 0.0f;

			Collision::Collide(velocity.Length() * 8.0, car->velocity.Length(), 5, 3, finalVelCar, finalVelCar2, 10);

			Vector3 vectorToCenter = car->position - position;
			Vector3 diff = vectorToCenter - forward;
			//std::cout << "distance to center: " << diff.Length() * Utility::Sign(diff) << std::endl;
			std::cout << velocity << std::endl;
	/*		std::cout << ai->forward.Dot(-forward) << std::endl;*/

			//std::cout << diff << std::endl;


			int t = 0;
			if (diff.x > 0)
			{
				t = (diff.z > 0) ? -1 : 1;
			}
			else
			{
				t = (diff.z > 0) ? 1 : -1;
			}
			
			if(finalVelCar != 0.0f)
				car->velocity = forward * finalVelCar2 * 0.60f;
			car->torqueRot = t * velocity.Length() * 12.0;

			float deltaRotY = Utility::Lerp(car->torqueRot, 0, 1.0 * dt);
			float targetRotY = Utility::Lerp(car->rotation.y,car->rotation.y + deltaRotY, 20.0f * dt);
			Vector3 deltaRotation = Vector3(0, deltaRotY, 0);

			// Rotate only if there is no collision
			if (Collision::checkCollisionR(this, deltaRotation, { "ground", "pad1" }).size() == 0) {
				car->torqueRot = deltaRotY;
				car->rotation.y = targetRotY;
			}

			collided = Collision::checkCollisionT(car, car->velocity, { "ground", "pad1" });
			if (car->velocity != Vector3(0, 0, 0) && collided.size() == 0)
			{
				car->position += car->velocity;
			}
			else {
				for (int i = 0; i < collided.size(); i++) {
					if (collided[i]->getType() == "environment") {
						Vector3 dirFromCar = collided[i]->position - car->position;
						if (velocity.Length() > 0.0f) {
							if (velocity.Dot(forward) >= 0.0f) {
								velocity.x = 0;
								velocity.z = 0;
								break;
							}
						}
					}
				}
			}
			

		}
	}

	
	float deltaRotY = Utility::Lerp(torqueRot , 0, 1.0 * dt);
	float targetRotY = Utility::Lerp(rotation.y, rotation.y + deltaRotY, 20.0f * dt);
	Vector3 deltaRotation = Vector3(0, deltaRotY, 0);

	// Rotate only if there is no collision
	if (Collision::checkCollisionR(this, deltaRotation, { "ground", "pad1" }).size() == 0) {
		torqueRot = deltaRotY;
		rotation.y = targetRotY;
	}

	collided = Collision::checkCollisionT(this, velocity, { "ground", "pad1" });
	if (velocity != Vector3(0, 0, 0) && collided.size() == 0)
	{
		position += velocity;
	}
	else {
		for (int i = 0; i < collided.size(); i++) {
			if (collided[i]->getType() == "environment") {
				Vector3 dirFromCar = collided[i]->position - position;
				if (velocity.Length() > 0.0f) {
					if (velocity.Dot(forward) >= 0.0f) {
						velocity.x = 0;
						velocity.z = 0;
						break;
					}
				}
			}
		}
	}

	Manager::getInstance()->getLevel()->getTree()->Update(this);
}


Vector3 Car::calcAcceleration(float accInput, float steerInput, float dt)
{

	Vector3 braking, acceleration, reverse, engineForward;

	// Steering of Car
	if (velocity.Length() > 0.01f)
		steerAngle = steerInput * 30;
	else if (accInput != -1)
		steerAngle = 0;

	if (accInput < 0.0f)
		steerAngle = -steerAngle;

	// Linearly interpolate for smooth transitions
	float newCurrentSteer = Utility::Lerp(currentSteer, currentSteer + steerAngle, (float)dt * 0.70f);
	
	float targetRotY = Utility::Lerp(rotation.y, -newCurrentSteer, 20.0f * dt);
	Vector3 deltaRotation = Vector3(0, -newCurrentSteer - rotation.y, 0);

	// Rotate only if there is no collision
	if (Collision::checkCollisionR(this, deltaRotation, { "ground", "pad1" }).size() == 0) {
		rotation.y = targetRotY;
		currentSteer = newCurrentSteer;
	}


	// Determine the steered forward of car
	float angle = 90.0f + currentSteer;
	float rad = Math::DegreeToRadian(angle);
	forward.x = cos(rad);
	forward.z = sin(rad);
	forward.Normalized();


	float velocityDir = Utility::Sign(velocity);

	// Interpolate forward and brake vectors.
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

	if (accInput == -1)
	{
		brakingAcceleration = 0.0f;
	}


	//if (velocity.Length() < 0.001f)
	//	velocity.SetZero();

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

	// Interpolate reverse vector
	reverseAcceleration = Utility::Lerp(reverseAcceleration, accInput, 10.0f * dt);
	reverse = reverseAcceleration * forward * 12.0 * dt;
	if (accInput != -1)
		reverse.SetZero();

	if (fabs(steerAmount) > 0.05f)
	{
		engineForward *= 0.7f;
	}

	// Determine forward acceleration
	acceleration = engineForward + braking + reverse;


	// Slow down acceleration while steering


	return acceleration * dt;
}


Vector3 Car::calcFriction(float accInput, float steerInput, float dt)
{
	Vector3 acceleration, friction, drag;
	Vector3 velNoY = velocity;
	velNoY.y = 0;

	if (velNoY.Length() > 0.0f && (velNoY.x != 0 || velNoY.z != 0))
	{
		
		if (accInput != -1) {
			drag = kDrag * velNoY.Length() * -velNoY.Normalized();
			friction = (kFriction * -velNoY.Normalized());
		}

		if (steerInput != 0)
			drag *= 1.2f;

	}


	acceleration = (friction + drag) * dt;
	if (acceleration.Length() > velNoY.Length())
		return Vector3(0,0,0);
	else
		return acceleration;
}


void Car::getVelocity(std::string& vel, Color& color) {

	Vector3 v = velocity * 170.0f;
	v.y = 0;
	vel = std::to_string(v.Length());
	vel = vel.substr(0, 2);
	if (vel[1] == '.')
		vel = vel[0];

	float ratioGreen = v.Length() / 70.0f;
	int ratioWhite = v.Length() / 20.0f;
	int ratioYellow = v.Length() / 40.0f;
	int ratioRed = v.Length() / 70.0f;

	if (ratioRed == 0)
	{
		if (ratioWhite == 0)
		{
			color.Set(1 - ratioWhite, 1 - ratioWhite, 1 - ratioWhite);
		}
		else
		{
			color.Set((ratioYellow == 0 ? 0 : ratioGreen), ratioGreen, 0);
		}
	}
	else
	{
		color.Set(1, 0, 0);
	}
}

// Friction

	//if (velocity.Length() != 0.0f)
	//{
	//	friction = (kFriction * -velocity.Normalized());
	//	if (accInput != -1)
	//		drag = kDrag * velocity.Length() * -velocity.Normalized();

	//	if (steerInput != 0)
	//		drag *= 1.2f;

	//}


// AI Waypoints
//if (accInput != previousInputs[0] && start)
//{
//	std::string text = "waypoint=pos:(" + std::to_string(position.x) + " " + std::to_string(position.y) + " " + std::to_string(position.z) + "),rot:" +
//		std::to_string(rotation.y - currentSteer) + ",acc:" + std::to_string(accInput) + ",steer:" + std::to_string(steerInput);
//	handle << text << std::endl;
//	std::cout << "Acc: " << position << ", Rot: " << rotation.y - currentSteer << "-> A: " << accInput << std::endl;
//}
//else if (steerInput != previousInputs[1] && start)
//{
//	std::string text = "waypoint=pos:(" + std::to_string(position.x) + " " + std::to_string(position.y) + " " + std::to_string(position.z) + "),rot:" +
//		std::to_string(rotation.y - currentSteer) + ",acc:" + std::to_string(accInput) + ",steer:" + std::to_string(steerInput);
//	handle << text << std::endl;
//	std::cout << "Steering: " << position << ", Rot: " << rotation.y - currentSteer << " -> S: " << steerInput << std::endl;
//}

//previousInputs[0] = accInput;
//previousInputs[1] = steerInput;
