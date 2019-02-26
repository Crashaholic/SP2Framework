#include "MovingObstacle.h"
#include "Utility.h"

MovingObstacle::MovingObstacle(const char* meshName, Primitive* primitive, Vector3 startPos, Vector3 endPos, unsigned int texID, DRAW_MODE drawMode):
	Mesh(meshName, primitive, texID, drawMode)
{
	this->startPos = startPos;
	this->endPos = endPos;
	target = endPos;
}


MovingObstacle::MovingObstacle()
{
}


MovingObstacle::~MovingObstacle()
{
}


void MovingObstacle::Update(double dt)
{
	Vector3 targetThisFrame = Utility::Lerp(position, target, 2.0 * dt);
	position = targetThisFrame;

	if ((target - position).Length() <= 0.1f)
	{
		if (target == startPos)
			target = endPos;
		else
			target = startPos;
	}
}