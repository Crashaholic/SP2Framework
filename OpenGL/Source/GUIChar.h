#ifndef GUICHAR_H
#define GUICHAR_H
class GUIChar
{
public:
	GUIChar(char id, float xPos, float yPos, float width, float height, float xOffset, float yOffset, float xAdvance);
	GUIChar();
	~GUIChar();

	char id;
	float xPos;
	float yPos;
	float width;
	float height;

	float xOffset;
	float yOffset;
	float xAdvance;


};

#endif