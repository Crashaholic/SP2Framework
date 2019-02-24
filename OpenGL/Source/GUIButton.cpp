#include "GUIButton.h"

GUIButton::GUIButton(std::string name, Vector3 pos, float rot, Vector3 scale, unsigned int normalTextureID, unsigned int hoverTextureID, std::string action)
{
	this->pos = pos;
	this->rot = rot;
	this->scale = scale;
	this->normalTextureID = normalTextureID;
	this->hoverTextureID = hoverTextureID;
	this->action = action;
	render = new IRender(name, pos, rot, scale, normalTextureID);
}

GUIButton::GUIButton(std::string name, Vector3 pos, float rot, Vector3 scale, Vector3 normalColor, float normalAlpha, Vector3 hoverColor, float hoverAlpha, std::string action)
{
	this->pos = pos;
	this->rot = rot;
	this->scale = scale;
	this->normalColor = normalColor;
	this->hoverColor = hoverColor;
	this->normalAlpha = normalAlpha;
	this->hoverAlpha = hoverAlpha;
	this->action = action;
	render = new IRender(name, pos, rot, scale, normalColor, normalAlpha);
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

bool GUIButton::isHover(double cursorX, double cursorY)
{
	double xPosP = scale.x + pos.x + 1;
	double xPosN = -scale.x + pos.x - 1;
	double yPosP = scale.y + pos.y + 1;
	double yPosN = -scale.y + pos.y - 1;

	return (cursorX < xPosP && cursorX > xPosN &&
		cursorY < yPosP && cursorY > yPosN);

}

bool GUIButton::checkStatus(double cursorX, double cursorY, double winWidth, double winHeight)
{

	if(isHover(cursorX, cursorY)){

		render->setTexture(hoverTextureID);
		render->setColor(hoverColor, hoverAlpha);

		if (Application::IsKeyPressed(VK_LBUTTON))
			return true;
	}
	else
	{
		render->setTexture(normalTextureID);
		render->setColor(normalColor, normalAlpha);
	}

	return false;
}

std::string GUIButton::getAction() const
{
	return action;
}
