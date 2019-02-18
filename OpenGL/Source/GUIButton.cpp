#include "GUIButton.h"

GUIButton::GUIButton(Vector3 pos, Vector3 rot, Vector3 scale, unsigned int normalTextureID, unsigned int hoverTextureID, buttonFunc funcName)
{
	this->pos = pos;
	this->rot = rot;
	this->scale = scale;
	this->normalTextureID = normalTextureID;
	this->hoverTextureID = hoverTextureID;
	this->funcName = funcName;
	render = new IRender(pos, rot, scale, normalTextureID);
}

GUIButton::GUIButton()
{

}

GUIButton::~GUIButton()
{

}

float GUIButton::runFunc(buttonFunc funcName)
{
	switch (funcName)
	{
	case NAME1:
		std::cout << "Mouse clicked!\n";
		return 0.0f;
		break;
	case NAME2:
		return 0.0f;
		break;
	case NAME3:
		return 0.0f;
		break;
	default:
		std::cout << "A button without a proper event has been clicked!\n";
		return 0.0f;
		break;
	}
}

void GUIButton::draw()
{
}

IRender* GUIButton::getIRender() const
{
	return render;
}

bool GUIButton::getHover() const
{
	return this->hover;
}

void GUIButton::setPos(Vector3 b)
{
	render->setPos(b);
}

void GUIButton::checkStatus(double cursorX, double cursorY, double winWidth, double winHeight)
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
		{
			runFunc(funcName);
		}
	
	}
	else
	{
		render->setTexture(normalTextureID);
	}

	

}