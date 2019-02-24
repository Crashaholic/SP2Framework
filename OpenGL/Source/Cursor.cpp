#include "Cursor.h"

Cursor::Cursor()
{
	lastX = lastY = 0;
	moveX = moveY = 0;
	currX = currY = 0;
	cursorGUI = new GUITexture("cursor",
		Vector3(0, 0, 0),
		0.0f,
		Vector3(16.f, 16.f, 16.f),
		LoadTGA("Image//cursor.tga"));

	cooldownTimer = -1.0f;
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

void Cursor::setOnCooldown(double duration)
{
	this->cooldownTimer = duration;
}

bool Cursor::updateVars(double newX, double newY, double winWidth, double winHeight, double dt)
{
	if (cooldownTimer >= 0.0f)
	{
		cooldownTimer -= dt;
		return false;
	}

	double mouseX = newX;
	double mouseY = newY;
	float mouseSensX = 1.5f;
	float mouseSensY = 1.25f;

	moveX = mouseX - lastX;
	moveY = lastY - mouseY;

	lastX = mouseX;
	lastY = mouseY;

	currX += moveX * mouseSensX;
	currY += moveY * mouseSensY;

	currX = Math::Clamp(currX, -winWidth / 2.0f, winWidth / 2.0f);
	currY = Math::Clamp(currY, -winHeight / 2.0f, winHeight / 2.0f);

	updateTexture();

	return true;
}

void Cursor::updateTexture()
{
	cursorGUI->setPos(Vector3(currX + 16.0f, currY - 16.0f));
}

GUITexture* Cursor::getGUITexture() const
{
	return cursorGUI;
}

