
#include "Mesh.h"
#include "GL\glew.h"
#include "LoadTGA.h"
#include "Primitives.h"
#include "Utility.h"
#include "Manager.h"
#include "Collision.h"
#include "LevitationPad.h"
#include "GUIManager.h"


/******************************************************************************/
/*!
\brief
Default constructor - generate VBO/IBO here

\param meshName - name of mesh
*/
/******************************************************************************/
Mesh::Mesh(const char* meshName, Primitive* primitive, unsigned int texID, bool collisionEnabled, bool gravityEnabled, std::string type, DRAW_MODE drawMode)
	: name(meshName)
	, mode(drawMode), textureID(texID), collisionEnabled(collisionEnabled), gravityEnabled(gravityEnabled), type(type)
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

Mesh::Mesh()
{

}

void Mesh::Init()
{
	velocity.SetZero();
	obb->setPos(position + Vector3(0, obb->getHalf().y, 0));
	defaultObb->setPos(position + Vector3(0, obb->getHalf().y, 0));
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

void Mesh::Update(double dt)
{
	if (Manager::getInstance()->getLevelName() == "game")
	{
		if (collisionEnabled)
		{
			if (gravityEnabled)
				onGroundCheck(dt);
			position += velocity;
			//Mesh* ground = Manager::getInstance()->getObject("ground");
			//float groundY = ground->position.y + ground->getOBB()->getHalf().y * 2;
		/*	if (position.y <= groundY)
				position.y = groundY;
			}*/
			Manager::getInstance()->getLevel()->getTree()->Update(this);
		}
	}
}

/******************************************************************************/
/*!
\brief
OpenGL render code
*/
/******************************************************************************/
void Mesh::Render(MS& modelStack, MS& viewStack, MS& projectionStack, ShaderProgram* shader)
{
	if (!isVisible) return;

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
	obb->setPos(position + Vector3(0, obb->getHalf().y, 0));


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

void Mesh::setVisible(bool visible) {
	isVisible = visible;
}

void Mesh::onGroundCheck(double dt)
{
	// Check whether position below the player collides with any object
	Vector3 grav = Vector3(0, -1.0f, 0);
	std::vector<Mesh*> collided = Collision::checkCollisionT(this, grav, {});

	// Check whether player is above a LevitationPad
	std::vector<Mesh*> collidePad = Collision::checkCollisionAbove(this, -35.0f, {});
	bool hasPad = std::find(collidePad.begin(), collidePad.end(), Manager::getInstance()->getLevel()->getObject("pad1")) != collidePad.end();


	// Nothing below the player -> Gravity pull
	if (!hasPad && collided.size() == 0)
	{
		//Vector3 target = velocity + grav * dt;
		//Mesh* ground = Manager::getInstance()->getObject("ground");
		//if (target.y <= ground->position.y) {
		//	position.y = ground->position.y + ground->getOBB()->getHalf().y * 2;
		//	velocity.y = 0;
		//}
		//else {
		//	velocity += grav * dt;
		//}
		velocity += grav * dt;
	}
	else
	{
		
		if (!hasPad && velocity.y < 0)
		{
			Mesh* collidee = collided[0];
			// Snap player to the ground if he is hovering slightly above due to inaccuracies in gravity collision
			float groundY = collidee->position.y + collidee->getOBB()->getHalf().y * 2;
			//float changeInY = position.y - groundY;

			//if (changeInY > 0.0f)
				position.y = groundY;

		}

		if (velocity.y < 0)
			velocity.y = 0;
		
	}

}

void Mesh::loadChildren(std::vector<std::string> names)
{



	for (int i = 0; i < (int)names.size(); i++)
	{
		std::string modelPath = "Models//" + names[i] + ".obj";
		Primitive* primitive = Primitives::loadModel(modelPath.c_str());
		Mesh* mesh = new Mesh(names[i].c_str(), primitive, textureID);
		children.push_back(mesh);
	}

}

OBB* Mesh::getOBB() {
	return obb;
}

void Mesh::Translate(MS& modelStack, float x, float y, float z) {
	position.Set(x, y, z);
	obb->setPosAxis(position + Vector3(0, obb->getHalf().y, 0), obb->getX(), obb->getY(), obb->getZ());
	modelStack.Translate(x, obb->getPos().y, z);
}

void Mesh::Rotate(MS& modelStack, float angle, float x, float y, float z) {

	//Vector3 rot = Vector3(x, y, z);
	//if (Collision::checkCollisionR(this, rot, {}).size() == 0) {

		if (x == 1) {
			obb->setPosAxis(position + Vector3(0, defaultObb->getHalf().y, 0), Utility::rotatePointByX(obb->getX(), angle),
				Utility::rotatePointByX(obb->getY(), angle),
				Utility::rotatePointByX(obb->getZ(), angle));
		}
		else if (y == 1) {
			obb->setPosAxis(position + Vector3(0, defaultObb->getHalf().y, 0), Utility::rotatePointByY(obb->getX(), angle),
				Utility::rotatePointByY(obb->getY(), angle),
				Utility::rotatePointByY(obb->getZ(), angle));
		}
		else if (z == 1) {
			obb->setPosAxis(position + Vector3(0, defaultObb->getHalf().y, 0), Utility::rotatePointByZ(obb->getX(), angle),
				Utility::rotatePointByZ(obb->getY(), angle),
				Utility::rotatePointByZ(obb->getZ(), angle));
		}
	/*}*/

	modelStack.Rotate(angle, x, y, z);

}

void Mesh::ResetOBB() {
	obb->setPosAxis(position + Vector3(0, defaultObb->getHalf().y, 0), defaultObb->getX(), defaultObb->getY(), defaultObb->getZ());

}

std::string Mesh::getType() {
	return type;
}