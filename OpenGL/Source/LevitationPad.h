#ifndef LEVITATION_PAD_H
#define LEVITATION_PAD_H

#include "Mesh.h"

class LevitationPad : public Mesh
{
public:
	LevitationPad(const char* meshName, Primitive* primitive, unsigned int texID = 0, float levitationForce=1.0f, DRAW_MODE drawMode = DRAW_TRIANGLES);
	LevitationPad();
	~LevitationPad();

	void Update(double dt);
	float getLevitationForce();

private:
	float levitationForce;
};

#endif