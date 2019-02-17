#include "LevitationPad.h"
#include "Collision.h"
#include "Manager.h"


LevitationPad::LevitationPad(const char* meshName, Primitive* primitive, unsigned int texID, float levitationForce, DRAW_MODE drawMode)
	:Mesh(meshName, primitive, texID, true, false, drawMode)
{
	position.Set(10.0f, 0.5f, 10.0f);
	this->levitationForce = levitationForce;
	obb->setHalf(Vector3(2.5f, 0.08f, 2.5f));
	defaultObb->setHalf(Vector3(2.5f, 0.08f, 2.5f));
}

LevitationPad::LevitationPad()
{
}


LevitationPad::~LevitationPad()
{
}

void LevitationPad::Update(double dt)
{
	Vector3 levitate = Vector3(0, levitationForce, 0);
	std::vector<Mesh*> collided = Collision::checkCollisionAbove(this, 100.0f, {"ground"});
	Manager* manager = Manager::getInstance();
	
	if (collided.size() > 0)
	{
		collided[0]->velocity += levitate * dt;
		std::cout << "." << std::endl;
	}

	Mesh::Update(dt);
}


