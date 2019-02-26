#ifndef MOVINGOBSTACLE_H
#define MOVINGOBSTACLE_H

#include "Mesh.h"

class MovingObstacle : public Mesh
{
public:
	MovingObstacle(const char* meshName, Primitive* primitive, Vector3 startPos, Vector3 endPos,
		unsigned int texID = 0, DRAW_MODE drawMode = DRAW_TRIANGLES);
	MovingObstacle();
	~MovingObstacle();
	void Update(double dt);
private:
	Vector3 target;
	Vector3 startPos;
	Vector3 endPos;
};


#endif
