
#include "Mesh.h"
#include "GL\glew.h"
#include "LoadTGA.h"
#include "Primitives.h"
#include "Utility.h"
#include "Manager.h"


/******************************************************************************/
/*!
\brief
Default constructor - generate VBO/IBO here

\param meshName - name of mesh
*/
/******************************************************************************/
Mesh::Mesh(const char* meshName, Primitive* primitive, unsigned int texID, bool collisionEnabled, DRAW_MODE drawMode)
	: name(meshName)
	, mode(drawMode) , textureID(texID)
{

	// Generate Buffers
	glGenBuffers(1, &vertexBuffer); 
	glGenBuffers(1, &indexBuffer); 


	// Bind & Buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, primitive->getVertices()->size() * sizeof(Vertex), &primitive->getVertices()->at(0), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, primitive->getIndices()->size() * sizeof(unsigned int), &primitive->getIndices()->at(0), GL_STATIC_DRAW);
	indexSize = primitive->getIndices()->size();

	obb = new OBB(Vector3(primitive->getWidth() * 0.5f, primitive->getHeight() * 0.5f, primitive->getDepth() * 0.5f));
	defaultObb = new OBB(Vector3(primitive->getWidth() * 0.5f, primitive->getHeight() * 0.5f, primitive->getDepth() * 0.5f));

}

Mesh::Mesh() {

}

void Mesh::InitTexture()
{
	if (textureID > 0)
	{
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 2, GL_FLOAT, false, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color) + sizeof(Vector3)));
	}
}

/******************************************************************************/
/*!
\brief
Destructor - delete VBO/IBO here
*/
/******************************************************************************/
Mesh::~Mesh()
{
	delete obb;
	delete defaultObb;

	if (textureID > 0)
	{
		glDeleteTextures(1, &textureID);
		glDisableVertexAttribArray(3);
	}

	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glDeleteBuffers(mode, &vertexBuffer);
	glDeleteBuffers(mode, &indexBuffer);

}

/******************************************************************************/
/*!
\brief
OpenGL render code
*/
/******************************************************************************/
void Mesh::Render(MS& modelStack, MS& viewStack, MS& projectionStack, ShaderProgram* shader)
{
	

	Mtx44 MVP, modelView, modelView_inverse_tranpose, model;
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	model = modelStack.Top();
	modelView = viewStack.Top() * modelStack.Top();

	shader->use();
	shader->setUniform("MVP", MVP);
	shader->setUniform("MV", modelView);
	shader->setUniform("model", model);
	shader->updateUniforms();

	// Enable and allocate attributes for Position, Color, Normal, Tex Coords
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), (void*) sizeof(Position));
	glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color)));


	InitTexture();
	obb->setPos(position);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	if (mode == DRAW_TRIANGLE_STRIP)
		glDrawElements(GL_TRIANGLE_STRIP, indexSize, GL_UNSIGNED_INT, 0);
	else if (mode == DRAW_LINES)
		glDrawElements(GL_LINES, indexSize, GL_UNSIGNED_INT, 0);
	else if (mode == DRAW_FAN)
		glDrawElements(GL_TRIANGLE_FAN, indexSize, GL_UNSIGNED_INT, 0);
	else
		glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, 0);

	if (textureID > 0) {
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisableVertexAttribArray(3);
	}

	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

}

std::vector<Mesh*>* Mesh::getChildren()
{
	return &children;
}

void Mesh::loadChildren(std::vector<std::string> names)
{



	for (int i = 0; i < (int)names.size(); i++)
	{
		std::string modelPath = "Models//" + names[i] + ".obj";
		Primitive* primitive = Primitives::loadModel(modelPath.c_str());
		Mesh* mesh = new Mesh(names[i].c_str(), primitive, textureID);
		children.push_back(mesh);
		obb->incrementSize(mesh->obb->getHalf());
	}

}

OBB* Mesh::getOBB() {
	return obb;
}

void Mesh::Translate(MS& modelStack, float x, float y, float z) {
	position.Set(x, y, z);
	obb->setPosAxis(position, obb->getX(), obb->getY(), obb->getZ());
	modelStack.Translate(x, y, z);
}

void Mesh::Rotate(MS& modelStack, float angle, float x, float y, float z) {


	if (x == 1) {
		obb->setPosAxis(position, Utility::rotatePointByX(obb->getX(), angle),
			Utility::rotatePointByX(obb->getY(), angle),
			Utility::rotatePointByX(obb->getZ(), angle));
	}
	else if (y == 1) {
		obb->setPosAxis(position, Utility::rotatePointByY(obb->getX(), angle),
			Utility::rotatePointByY(obb->getY(), angle),
			Utility::rotatePointByY(obb->getZ(), angle));
	}
	else if (z == 1) {
		obb->setPosAxis(position, Utility::rotatePointByZ(obb->getX(), angle),
			Utility::rotatePointByZ(obb->getY(), angle),
			Utility::rotatePointByZ(obb->getZ(), angle));
	}

	modelStack.Rotate(angle, x, y, z);

}

void Mesh::ResetOBB() {
	obb->setPosAxis(position, defaultObb->getX(), defaultObb->getY(), defaultObb->getZ());

}
