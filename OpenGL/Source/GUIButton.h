#ifndef GUI_BUTTON
#define GUI_BUTTON
#include "IRender.h"

class GUIButton
{
public:
	GUIButton(std::string name, Vector3 pos, float rot, Vector3 scale, unsigned int normalTextureID, unsigned int hoverTextureID, std::string action);
	GUIButton(std::string name, Vector3 pos, float rot, Vector3 scale, Vector3 normalColor, float normalAlpha, Vector3 hoverColor, float hoverAlpha, std::string action);
	GUIButton();
	~GUIButton();

	IRender* getIRender() const;
	bool checkStatus(double cursorX, double cursorY, double winWidth, double winHeight);
	std::string getAction() const;
	void draw();
	void setPos(Vector3 b);
	bool isHover(double cursorX, double cursorY);

private:
	unsigned int normalTextureID;
	unsigned int hoverTextureID;
	IRender* render;
	Vector3 pos, scale;
	Vector3 normalColor, hoverColor;
	float normalAlpha;
	float hoverAlpha;
	float rot; 
	std::string action;
};

#endif