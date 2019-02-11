#ifndef GUIFONT_H
#define GUIFONT_H

#include "GUIChar.h"

#include "Vertex.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <map>

struct GUIGlyph {
	Vector3 position;
	TexCoord texCoord;
};

enum TextAlignment {
	TEXT_ALIGN_LEFT,
	TEXT_ALIGN_RIGHT,
	TEXT_ALIGN_TOP,
	TEXT_ALIGN_MIDDLE,
	TEXT_ALIGN_BOTTOM,
	TEXT_ALIGN_BOTTOMLEFT,
};

class GUIFont
{
public:
	GUIFont(const char* filePath, const char* texturePath);
	GUIFont();
	~GUIFont();
	GUIChar* getChar(char c);
	unsigned int getVBO();
	unsigned int getTexture();
	void load(const char* path);
	void createDataFromText(std::vector<GUIGlyph>& glyphs, float xPos, float yPos, std::string text, float fontSize=1.0f, TextAlignment align=TEXT_ALIGN_LEFT);
private:
	unsigned int texture;
	unsigned int VBO;
	float getValue(std::string input);
	std::map<int, GUIChar*> data;
	//GUIChar* data[256];
};

#endif