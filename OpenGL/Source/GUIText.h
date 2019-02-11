#ifndef GUITEXT_H
#define GUITEXT_H

#include "GUIFont.h"



class GUIText
{
public:
	GUIText(std::vector<GUIGlyph> glyphs, float xPos, float yPos, unsigned int fontTexture, float fontSize=1.0f, Color color = Color(1, 1, 1));
	GUIText();
	~GUIText();

	void render();
	std::vector<GUIGlyph>* getGlyphs();
	
private:

	float xPos;
	float yPos;
	float fontSize;
	Color color;

	unsigned int VAO;
	unsigned int VBO;
	unsigned int fontTexture;
	std::vector<GUIGlyph> glyphs;
};

#endif
