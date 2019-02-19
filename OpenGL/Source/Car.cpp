#include "Car.h"
#include "Application.h"
#include "GUIManager.h"
#include "Utility.h"
#include "Collision.h"
#include "Manager.h"
#include "Player.h"
#include "AICar.h"

Car::Car(const char* meshName, Primitive* primitive, unsigned int texID, DRAW_MODE drawMode)
	: Mesh(meshName, primitive, texID, true, true, "car", drawMode)
{



	//position.Set(0.0f, 20.0f, 10.0f);
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

	previousInputs[0] = previousInputs[1] = 0;

	//obb->setHalf(Vector3(2.192, 1.2445, 4.289));
	//defaultObb->setHalf(Vector3(2.192, 1.2445, 4.289));
	handle.open("waypoints.txt");
	mode = PHYSICS_CAR;

	xDelta = 0.0f;
	zDelta = 0.0f;
	thrusters = 300.0f;
	thrust = 0.0f;
}

Car::Car()
{

}

Car::~Car()
{
	handle.close();
}

void Car::setOccupied(bool isOccupied)
{
	this->isOccupied = isOccupied;
}


void Car::Update(double dt)
{

	if (name == "ground" || name.substr(0, 4) == "car_") return;
	Vector3 grav = Vector3(0, -1.0f, 0);
	std::vector<Mesh*> collided = Collision::checkCollisionT(this, grav, {});

	std::vector<Mesh*> collidePath = Collision::checkCollisionAbove(this, -20.0f, {});
	bool hasPad = std::find(collidePath.begin(), collidePath.end(), Manager::getInstance()->getObject("pad1")) != collidePath.end();

	if (!hasPad && std::find(collided.begin(), collided.end(), Manager::getInstance()->getObject("ground")) == collided.end()) {
		velocity += grav * dt;
	}
	else {

		if (!hasPad) {
			Vector3 ground = Manager::getInstance()->getObject("ground")->getOBB()->getPos() + Manager::getInstance()->getObject("ground")->getOBB()->getHalf().y;
			Vector3 distance = obb->getPos() - obb->getHalf().y - ground;
			distance.x = 0;
			distance.z = 0;
			if (distance.Length() > 0.0f) {
				position.y = Manager::getInstance()->getObject("ground")->position.y + Manager::getInstance()->getObject("ground")->getOBB()->getHalf().y;
			}
		}

		if (velocity.y < 0)
			velocity.y = 0;
	}
	position.y += velocity.y;

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

		if (Application::IsKeyPressed('M')) {
			mode = PHYSICS_PLANE;
		}

		velocity += updatePosition(accInput, steerInput, dt);
		std::vector<Mesh*> collided = Collision::checkCollisionT(this, velocity, { "ground", "pad1" });
		if (velocity != Vector3(0, 0, 0) && collided.size() == 0) {
			position += velocity;
		}

	
		

	}

	//std::vector<Mesh*> collided = Collision::checkCollisionT(this, velocity, { "ground", "pad1" });
	//if (velocity != Vector3(0, 0, 0) && collided.size() == 0) {
	//	position += velocity;
	//}


	//std::vector<Mesh*> collided = Collision::checkCollisionType(this, velocity, "ai");
	//if (velocity != Vector3(0, 0, 0) && collided.size() != 0) {
	//	for (int i = 0; i < collided.size(); i++) {
	//		AICar* ai = dynamic_cast<AICar*>(collided[i]);
	//		//std::cout << "AI's velocity: " << ai->velocity.Length() << std::endl;
	//		//std::cout << "Car's velocity: " << velocity.Length() << std::endl;

	//		// Find difference in their forward 
	//		float kForwardDiff = abs(ai->forward.Dot(-forward));
	//		float finalVelCar = 0.0f;
	//		float finalVelAI = 0.0f;
	//		Collision::Collide(velocity.Length(), ai->velocity.Length(), 5, 3, finalVelCar, finalVelAI, 10);
	//		std::cout << "BEFORE: " << std::endl;
	//		std::cout << "AI's velocity: " << ai->velocity.Length() << std::endl;
	//		std::cout << "Car's velocity: " << velocity.Length() << std::endl;
	//		std::cout << "AFTER: " << std::endl;
	//		std::cout << "AI's velocity: " << finalVelAI << std::endl;
	//		std::cout << "Car's velocity: " << finalVelCar << std::endl;

	//		//if (ai->velocity.Length() != 0) {
	//		//	float ratio = finalVelAI / ai->velocity.Length();
	//		//	std::cout << "Ratio: " << ratio << std::endl;
	//		//	ai->velocity *= ratio;
	//		//}
	//		//else {
	//		//	velocity = forward * finalVelCar;
	//		//	ai->position += ai->forward * finalVelAI;
	//		//}
	//		

	//	}
	//}



	


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

	float angle = 90.0f + currentSteer;
	float rad = Math::DegreeToRadian(angle);
	forward.x = cos(rad);
	forward.z = sin(rad);
	forward.Normalized();

	rotation.y = -currentSteer;

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
		std::string text = "waypoint=pos:(" + std::to_string(position.x) + " " + std::to_string(position.y) + " " + std::to_string(position.z) + "),rot:" +
			std::to_string(rotation.y - currentSteer) + ",acc:" + std::to_string(accInput) + ",steer:" + std::to_string(steerInput);
		handle << text << std::endl;
		std::cout << "Acc: " << position << ", Rot: " << rotation.y - currentSteer << "-> A: " << accInput << std::endl;
	}
	else if (steerInput != previousInputs[1] && start)
	{
		std::string text = "waypoint=pos:(" + std::to_string(position.x) + " " + std::to_string(position.y) + " " + std::to_string(position.z) + "),rot:" +
			std::to_string(rotation.y - currentSteer) + ",acc:" + std::to_string(accInput) + ",steer:" + std::to_string(steerInput);
		handle << text << std::endl;
		std::cout << "Steering: " << position << ", Rot: " << rotation.y - currentSteer << " -> S: " << steerInput << std::endl;
	}

	

	previousInputs[0] = accInput;
	previousInputs[1] = steerInput;
	return acceleration * dt;
}


