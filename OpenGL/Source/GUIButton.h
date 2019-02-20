#ifndef GUI_BUTTON
#define GUI_BUTTON
#include "IRender.h"

class GUIButton
{
public:
	GUIButton(Vector3 pos, float rot, Vector3 scale, 
		unsigned int normalTextureID, unsigned int hoverTextureID,
		std::string name);
	GUIButton(Vector3 pos, float rot, Vector3 scale,
		Vector3 normalColor, float normalAlpha, Vector3 hoverColor, float hoverAlpha,
		std::string name);
	GUIButton();
	~GUIButton();

	IRender* getIRender() const;
	bool checkStatus(double cursorX, double cursorY, double winWidth, double winHeight);
	std::string getName() const;
	void draw();
	void setPos(Vector3 b);

private:
	unsigned int normalTextureID;
	unsigned int hoverTextureID;
	IRender* render;
	Vector3 pos, scale;
	Vector3 normalColor, hoverColor;
	float normalAlpha;
	float hoverAlpha;
	float rot; 
	std::string name;
};

#endif