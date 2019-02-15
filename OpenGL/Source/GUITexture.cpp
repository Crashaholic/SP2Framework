#include "GUITexture.h"

/**
* 
*/

GUITexture::GUITexture(Vector3 pos, Vector3 rot, Vector3 scale, unsigned int textureID)
{
	quad = Primitives::generateQuad(Color(1, 0, 1));
	render = new IRender(pos, rot, scale, *quad->getVertices(), *quad->getIndices(), textureID);
}

GUITexture::~GUITexture()
{

}

void GUITexture::setTexture(unsigned int existingTexture)
{
	render->setTexture(existingTexture);
}

void GUITexture::setTexture(const char* newTexture)
{
	render->setTexture(newTexture);
}

IRender* GUITexture::getIRender() const
{
	return this->render;
}

void GUITexture::draw() 
{
	// empty
}

void GUITexture::SetPos(Vector3 b)
{
	pos = b;
	render->SetPos(pos);
}
