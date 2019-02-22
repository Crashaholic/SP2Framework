#ifndef GUI_TEXTURE
#define GUI_TEXTURE

#include "IRender.h"

/**
 *	GUITexture element. Meant to display textures as a GUI.
 */
class GUITexture
{
public:
	GUITexture(Vector3 pos, float rot, Vector3 scale, unsigned int textureID);
	GUITexture(Vector3 pos, float rot, Vector3 scale, Vector3 color, float alpha);
	GUITexture() {};
	~GUITexture();

	void setTexture(unsigned int existingTexture);
	void setTexture(const char* newTexture);
	void setColor(Vector3 color, float alpha);
	IRender* getIRender() const;
	void draw();
	void setPos(Vector3 b);
private:
	IRender* render;
	Vector3 pos, scale;
	float rot;
};

#endif