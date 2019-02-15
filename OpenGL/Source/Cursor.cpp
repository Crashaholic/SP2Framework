#include "Cursor.h"

Cursor::Cursor()
{
	lastX = lastY = 0;
	moveX = moveY = 0;
	currX = currY = 0;
	cursorGUI = new GUITexture(
		Vector3(0, 0, 0),
		Vector3(0, 0, 0),
		Vector3(1, 1, 1),
		LoadTGA("Image//cursor.tga"));
}

double Cursor::getX()
{
	return lastX;
}

double Cursor::getY()
{
	return lastY;
}

double Cursor::getMoveX()
{
	return moveX;
}

double Cursor::getMoveY()
{
	return moveY;
}

void Cursor::updateVars(double newX, double newY)
{
	double MouseX = newX;
	double MouseY = newY;

	moveX = MouseX - lastX;
	moveY = lastY - MouseY;

	lastX = MouseX;
	lastY = MouseY;

	currX += moveX;
	currY += moveY;
}

void Cursor::updateTexture()
{
	cursorGUI->SetPos(Vector3(currX, currY));
}

GUITexture* Cursor::getGUITexture() const
{
	return cursorGUI;
}