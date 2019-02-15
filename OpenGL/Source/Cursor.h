#pragma once
#include "GUITexture.h"
#include "LoadTGA.h"

class Cursor
{
private:
	double lastX, lastY;
	double moveX, moveY;
	double currX, currY;
	GUITexture* cursorGUI;
public:
	Cursor();
	double getX();
	double getY();
	double getMoveX();
	double getMoveY();

	void updateVars(double newX, double newY);
	void updateTexture();

	GUITexture* getGUITexture() const;

};
