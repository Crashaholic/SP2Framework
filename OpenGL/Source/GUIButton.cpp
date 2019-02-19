#include "GUIButton.h"

GUIButton::GUIButton(Vector3 pos, Vector3 rot, Vector3 scale, unsigned int normalTextureID, unsigned int hoverTextureID, std::string name)
{
	this->pos = pos;
	this->rot = rot;
	this->scale = scale;
	this->normalTextureID = normalTextureID;
	this->hoverTextureID = hoverTextureID;
	this->name = name;
	render = new IRender(pos, rot, scale, normalTextureID);
}

GUIButton::GUIButton()
{

}

GUIButton::~GUIButton()
{

}

void GUIButton::draw()
{
}

IRender* GUIButton::getIRender() const
{
	return render;
}

void GUIButton::setPos(Vector3 b)
{
	render->setPos(b);
}

bool GUIButton::checkStatus(double cursorX, double cursorY, double winWidth, double winHeight)
{
	double xPosP = scale.x + pos.x + 1;
	double xPosN = -scale.x + pos.x - 1;
	double yPosP = scale.y + pos.y + 1;
	double yPosN = -scale.y + pos.y - 1;

	if (cursorX < xPosP &&
		cursorX > xPosN &&
		cursorY < yPosP &&
		cursorY > yPosN)
	{
		render->setTexture(hoverTextureID);
		if (Application::IsKeyPressed(VK_LBUTTON))
			return true;
		else
			return false;
	}
	else
		render->setTexture(normalTextureID);

	

}

std::string GUIButton::getName() const
{
	return this->name;
}
