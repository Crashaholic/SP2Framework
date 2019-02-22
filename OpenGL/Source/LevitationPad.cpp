#include "LevitationPad.h"
#include "Collision.h"
#include "Manager.h"


LevitationPad::LevitationPad(const char* meshName, Primitive* primitive, unsigned int texID, float levitationForce, DRAW_MODE drawMode)
	:Mesh(meshName, primitive, texID, true, true, "pad", drawMode)
	, levitationForce(levitationForce)
{

}

LevitationPad::LevitationPad()
{
}


LevitationPad::~LevitationPad()
{
}

float LevitationPad::getLevitationForce() {
	return levitationForce;
}

void LevitationPad::Update(double dt)
{


	Mesh::Update(dt);
	Vector3 levitate = Vector3(0, levitationForce, 0) * 4.15 * dt;
	std::vector<Mesh*> collided = Collision::checkCollisionAbove(this, 35.0f, {"ground"});
	Manager* manager = Manager::getInstance();
	
	for (int i = 0; i < collided.size(); i++) {
		Mesh* m = collided[i];
		if (abs(m->position.y - position.y + (getOBB()->getHalf().y * 2)+ levitate.y) < 35.0f) {
			m->velocity.y = levitate.y;
		}
		else {
			m->position.y = position.y + 35.0f;
			m->velocity.y = 0.0;
		}
	}



}


