
#include "Mesh.h"
#include "GL\glew.h"
#include "LoadTGA.h"
#include "Primitives.h"
#include "Utility.h"
#include "CollisionManager.h"
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
		children.push_back(new Mesh(names[i].c_str(), Primitives::loadModel(modelPath.c_str()), textureID));

	}

}




