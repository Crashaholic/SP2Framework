#include "GUITexture.h"

/**
* 
*/

GUITexture::GUITexture(std::string name, Vector3 pos, float rot, Vector3 scale, unsigned int textureID)
{
	render = new IRender(name, pos, rot, scale, textureID);
}

GUITexture::GUITexture(std::string name, Vector3 pos, float rot, Vector3 scale, Vector3 color, float alpha)
{
	render = new IRender(name, pos, rot, scale, color, alpha);
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

void GUITexture::setColor(Vector3 color, float alpha)
{
	render->setColor(color, alpha);
}

void GUITexture::setPos(Vector3 b)
{
	pos = b;
	render->setPos(pos);
}
