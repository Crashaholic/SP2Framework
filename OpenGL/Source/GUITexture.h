#ifndef GUI_TEXTURE
#define GUI_TEXTURE

#include "IRender.h"

/**
 *	GUITexture element. Meant to display textures as a GUI.
 */
class GUITexture
{
public:
	GUITexture(Vector3 pos, Vector3 rot, Vector3 scale, unsigned int textureID);
	GUITexture() {};
	~GUITexture();

	void setTexture(unsigned int existingTexture);
	void setTexture(const char* newTexture);
	IRender* getIRender() const;
	void draw();
	void SetPos(Vector3 b);
private:
	IRender* render;
	Primitive* quad;
	Vector3 pos, rot, scale;
};

#endif