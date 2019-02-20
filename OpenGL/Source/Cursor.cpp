#include "Cursor.h"

Cursor::Cursor()
{
	lastX = lastY = 0;
	moveX = moveY = 0;
	currX = currY = 0;
	cursorGUI = new GUITexture(
		Vector3(0, 0, 0),
		0.0f,
		Vector3(16.f, 16.f, 16.f),
		LoadTGA("Image//cursor.tga"));
}

double Cursor::getX()
{
	return currX;
}

double Cursor::getY()
{
	return currY;
}

double Cursor::getMoveX()
{
	return moveX;
}

double Cursor::getMoveY()
{
	return moveY;
}

void Cursor::updateVars(double newX, double newY, double winWidth, double winHeight)
{
	double MouseX = newX;
	double MouseY = newY;
	float mouseSensX = 1.5f;
	float mouseSensY = 1.25f;

	moveX = MouseX - lastX;
	moveY = lastY - MouseY;

	lastX = MouseX;
	lastY = MouseY;
	
	currX += moveX * mouseSensX;
	currY += moveY * mouseSensY;

	currX = Math::Clamp(currX, -winWidth / 2.0f, winWidth / 2.0f);
	currY = Math::Clamp(currY, -winHeight / 2.0f, winHeight / 2.0f);
	//std::cout << "cursorX: " << currX << " | cursorY: " << currY << '\n';
}

void Cursor::updateTexture()
{
	cursorGUI->setPos(Vector3(currX, currY));
}

GUITexture* Cursor::getGUITexture() const
{
	return cursorGUI;
}

