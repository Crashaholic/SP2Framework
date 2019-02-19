#pragma once
#include "GUITexture.h"
#include "LoadTGA.h"
#include <iostream>

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

	void updateVars(double newX, double newY, double winWidth, double winHeight);
	void updateTexture();

	GUITexture* getGUITexture() const;

};
