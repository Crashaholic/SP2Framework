#include "LevitationPad.h"
#include "Collision.h"
#include "Manager.h"


LevitationPad::LevitationPad(const char* meshName, Primitive* primitive, unsigned int texID, float levitationForce, DRAW_MODE drawMode)
	:Mesh(meshName, primitive, texID, true, false, "pad", drawMode)
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
	Vector3 levitate = Vector3(0, levitationForce, 0) * 5.0 * dt;
	std::vector<Mesh*> collided = Collision::checkCollisionAbove(this, 20.0f, {"ground"});
	Manager* manager = Manager::getInstance();
	
	for (int i = 0; i < collided.size(); i++) {
		Mesh* m = collided[i];
		if (abs(m->position.y - position.y + levitate.y) < 20.0f) {
			
			m->position += levitate;
		}
		else {
			m->position.y = position.y + 20.0f;
			m->velocity.y = 0.0;
		}
	}

	//if (collided.size() > 0 && collided[0]->position.y < 50.0f)
	//{
	//	//collided[0]->position += levitate;
	//	collided[0]->position += levitate * dt;
	//	std::cout << "." << std::endl;

	//}
	


}


