#ifndef GUI_BUTTON
#define GUI_BUTTON
#include "IRender.h"

class GUIButton
{
public:
	GUIButton(Vector3 pos, Vector3 rot, Vector3 scale, 
		unsigned int normalTextureID, unsigned int hoverTextureID,
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
	Vector3 pos, rot, scale;
	std::string name;
};

#endif