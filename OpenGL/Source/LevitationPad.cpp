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

	//Mesh::Update(dt);
	Vector3 levitate = Vector3(0, levitationForce, 0) * 6.0 * dt;
	std::vector<Mesh*> collided = Collision::checkCollisionAbove(this, 80.0f, {"ground"});
	Manager* manager = Manager::getInstance();
	
	for (int i = 0; i < collided.size(); i++) {
		Mesh* m = collided[i];
		if (abs(m->position.y - position.y + (getOBB()->getHalf().y * 2)+ levitate.y) < 80.0f) {
			m->velocity.y += levitate.y * dt;
		}
		else {
			m->position.y = position.y + 80.0f;
			m->velocity.y = 0.0;
		}
	}



}


