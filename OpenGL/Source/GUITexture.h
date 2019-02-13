#ifndef GUI_TEXTURE
#define GUI_TEXTURE

#include "IRender.h"

class GUITexture
{
public:
	GUITexture(Vector3 pos, Vector3 rot, Vector3 scale);
	GUITexture(const char* path);
	GUITexture(const char* path);
	void setTexture(unsigned int existingTexture);
	void setTexture(const char* newTexture);
	GUITexture(unsigned int existingTexture);
	~GUITexture();
	
	void draw();
	IRender* render;
	Primitive* quad;
};

#endif