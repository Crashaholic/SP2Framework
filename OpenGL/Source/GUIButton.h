#ifndef GUI_BUTTON
#define GUI_BUTTON
#include "IRender.h"

class GUIButton
{
public:
	enum buttonFunc
	{
		NAME1,
		NAME2,
		NAME3,
		TOTAL
	};

	GUIButton(Vector3 pos, Vector3 rot, Vector3 scale, 
		unsigned int normalTextureID, unsigned int hoverTextureID,
		buttonFunc funcName);
	GUIButton();
	~GUIButton();

	IRender* getIRender() const;
	float runFunc(buttonFunc funcName); 
	void draw();
	bool getHover() const;
	void setPos(Vector3 b);
	void checkStatus(double cursorX, double cursorY, double winWidth, double winHeight);

private:
	bool hover;
	unsigned int normalTextureID;
	unsigned int hoverTextureID;
	IRender* render;
	Vector3 pos, rot, scale;
	buttonFunc funcName;
};

/*
public:
	GUITexture(Vector3 pos, Vector3 rot, Vector3 scale, unsigned int textureID);
	GUITexture() {};
	~GUITexture();

	void setTexture(unsigned int existingTexture);
	void setTexture(const char* newTexture);
	IRender* getIRender() const;
	void draw();
	void setPos(Vector3 b);
private:
	IRender* render;
	Vector3 pos, rot, scale;
*/

#endif