#include "GUIChar.h"


GUIChar::GUIChar(char id, float xPos, float yPos, float width, float height, float xOffset, float yOffset, float xAdvance)
{
	this->id = id;
	this->xPos = xPos;
	this->yPos = yPos;
	this->width = width;
	this->height = height;
	this->xOffset = xOffset;
	this->yOffset = yOffset;
	this->xAdvance = xAdvance;
}

GUIChar::GUIChar()
{
	id = 0;
	xPos = width = height = xOffset = yOffset = xAdvance = -1;
}


GUIChar::~GUIChar()
{
}
