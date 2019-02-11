#ifndef MESH_H
#define MESH_H

#include <string>
#include "Vertex.h"
#include "Material.h"
#include "Primitives.h"
#include "MatrixStack.h"
#include "Transformation.h"
#include "ShaderProgram.h"
#include "Collision.h"

/******************************************************************************/
/*!
		Class Mesh:
\brief	To store VBO (vertex & color buffer) and IBO (index buffer)
*/
/******************************************************************************/
class Mesh
{
public:




	enum DRAW_MODE
	{
		DRAW_TRIANGLES, //default mode
		DRAW_TRIANGLE_STRIP,
		DRAW_LINES,
		DRAW_FAN,
		DRAW_MODE_LAST,
	};
	Mesh(const char* meshName, Primitive* primitive, unsigned int texID=0, bool collisionEnabled = false, DRAW_MODE drawMode=DRAW_TRIANGLES);
	Mesh();
	~Mesh();
	void loadChildren(std::vector<std::string> names);

	const std::string name;
	DRAW_MODE mode;
	unsigned int vertexBuffer;
	unsigned int indexBuffer;
	unsigned int indexSize;
	unsigned int textureID;
	bool hasResetCollider;

	Vector3 position;
	Vector3 rotation;

	Material material;
	std::vector<Mesh*>* getChildren();
	OBB* getOBB();
	std::vector<Transformation*> transformations;

	virtual void Render(MS& modelStack, MS& viewStack, MS& projectionStack, ShaderProgram* shader);

	void Translate(MS& modelStack, float x, float y, float z);
	void Scale(MS& modelStack, float x, float y, float z);
	void Rotate(MS& modelStack, float angle, float x, float y, float z);
	void ResetOBB();



private:
	virtual void InitTexture();

protected:
	OBB* obb;
	OBB* defaultObb;
	std::vector<Mesh*> children;

};

#endif