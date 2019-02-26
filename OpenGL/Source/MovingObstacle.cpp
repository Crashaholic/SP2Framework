#include "MovingObstacle.h"
#include "Utility.h"
#include "Collision.h"

MovingObstacle::MovingObstacle(const char* meshName, Primitive* primitive, Vector3 startPos, Vector3 endPos, float moveRate, 
	unsigned int texID, DRAW_MODE drawMode): Mesh(meshName, primitive, texID, true, false, "movingobstacle")
{
	this->startPos = startPos;
	this->endPos = endPos;
	this->moveRate = moveRate;
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
	Vector3 targetThisFrame = Utility::Lerp(position, target, moveRate * dt);

	Vector3 delta = 1.2f * (targetThisFrame - position);
	if (Collision::checkCollisionType(this, delta, { "car" }).size() == 0) {

		position = targetThisFrame;

		if ((target - position).Length() <= 0.5f)
		{
			if (target == startPos)
				target = endPos;
			else
				target = startPos;
		}
	}
}