#include "Car.h"
#include "Application.h"
#include "GUIManager.h"
#include "Manager.h"

Car::Car(const char* meshName, Primitive* primitive, unsigned int texID, DRAW_MODE drawMode)
	: Mesh(meshName, primitive, texID, true, drawMode) {


	position.Set(10.0f, 0.0f, 0.0f);
	rotation.Set(0, 0, 0);
	velocity.Set(0, 0, 0);
	forward.Set(0, 0, 1);

	steeringWheelAngle = 0.0f;
	steerAngle = 0.0f;
	currentSteer = 0.0f;
	wheelAngle = 0.0f;
	friction = 0.0f;

	kAcceleration = 4.0f;
	kSteer = 20.0f;
	kSteerLerp = 30.0f;
	kFriction = 0.10f;


	maxSpeed = 80.0f;
	showLight = false;
}


Car::~Car()
{
}


void Car::Update(double dt)
{
	float accInput = 0.0f;
	float steerInput = 0.0f;

	// Input
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



	//// Rotating the Steering Wheel
	//steeringWheelAngle += (float)(steerInput * dt * 40.0f);
	//steeringWheelAngle += -sign(steeringWheelAngle) * 0.4f;
	//steeringWheelAngle = clamp(-32, 32, steeringWheelAngle);
	//if (steeringWheelAngle >= -0.1f && steeringWheelAngle <= 0.1f)
	//	steeringWheelAngle = 0.0f;


	//// Rotating Wheels
	//if (velocity.Length() > 0)
	//{
	//	wheelAngle += accInput * 80.0f * (float)dt;
	//	if (wheelAngle >= 360.0f)
	//		wheelAngle = 0.0f;
	//	else if (wheelAngle <= 0.0f)
	//		wheelAngle = 360.0f;
	//}

	//// Steering Car
	//if (accInput != 0.0f)
	//	steerAngle = steerInput * kSteer;
	//else
	//	steerAngle = 0.0f;

	//if (accInput < 0.0f)
	//	steerAngle = -steerAngle;

	//currentSteer = lerp(currentSteer, currentSteer + steerAngle, (float)dt * 2.0f);

	//// Updating Car's Forward
	//float rad = Math::DegreeToRadian(rotation.y + 90.0f + currentSteer);
	//forward.x = cos(rad);
	//forward.z = sin(rad);
	//forward.Normalized();

	//// Calculate acceleration
	//acceleration = forward * accInput * kAcceleration;


	//// Friction
	//friction = -velocity * kFriction;
	//velocity += friction * 80.0f * (float)dt;

	//velocity += acceleration * (float)dt;





}

void Car::Render(MS& modelStack, MS& viewStack, MS& projectionStack, ShaderProgram* shader)
{



	// Apply Transformations
	Translate(modelStack, position.x, position.y, position.z);
	Rotate(modelStack, rotation.x, 1, 0, 0);
	Rotate(modelStack, rotation.y, 0, 1, 0);
	Rotate(modelStack, -currentSteer, 0, 1, 0);
	Rotate(modelStack, rotation.z, 0, 0, 1);


	// Render
	Mesh::Render(modelStack, viewStack, projectionStack, shader);




	// Draw Velocity
	int vSpeed = (int)(velocity.Length() * 161);
	GUIManager::getInstance()->renderText("game", 10, 590, "V: " + std::to_string(vSpeed) + " mph", 0.35f, Color(1, 0, 0), TEXT_ALIGN_BOTTOMLEFT);
}


float Car::sign(float n)
{
	if (n > 0)
		return 1;
	else if (n < 0)
		return -1;
	else
		return 0;
}

float Car::lerp(float start, float end, float percent)
{
	return (start + percent * (end - start));
}

Vector3 Car::lerp(Vector3 start, Vector3 end, float percent)
{
	return (start + percent * (end - start));
}

float Car::clamp(float min, float max, float n)
{
	if (n < min)
		return min;
	else if (n > max)
		return max;
	else
		return n;
}