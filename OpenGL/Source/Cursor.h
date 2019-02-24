#ifndef CURSOR_H
#define CURSOR_H

#include "GUITexture.h"
#include "LoadTGA.h"
#include <iostream>

class Cursor
{


public:
	Cursor();
	double getX();
	double getY();
	double getMoveX();
	double getMoveY();

	void setOnCooldown(double duration);
	bool updateVars(double newX, double newY, double winWidth, double winHeight, double dt);
	void updateTexture();

	GUITexture* getGUITexture() const;

private:
	double cooldownTimer;
	double lastX, lastY;
	double moveX, moveY;
	double currX, currY;
	GUITexture* cursorGUI;
};

#endif