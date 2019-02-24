#include "GUIFont.h"
#include "GL\glew.h"
#include "LoadTGA.h"
#include "Utility.h"
#include "Application.h"


GUIFont::GUIFont(const char* path, const char* texturePath)
{

	texture = LoadTGA(texturePath);

	load(path);
}

GUIFont::GUIFont()
{

}


GUIFont::~GUIFont()
{
	for (auto const& d : data)
		if (d.second != nullptr)
			delete d.second;
}

/*

Generates the necessary vertices and tex coordinates for a string of text
to be rendered

Things to take note:
-> Texture coordinates range (0..1) , Bottom left is (0,0)
-> Vertex coordinates exist in Normalized Device Coordinates (NDC) range (-1..1), Center is (0,0)

========================================
To change from 0..screen width to NDC
-> ndc = ((s-1)/ (screen width / 2)) - 1

To change from 0..screen width to 0..1
-> ndc = s / screen_width
========================================

For each character (glyph), a set of 6 vertices (2 triangles) is generated along with 4 texture coordinates.

Vertices:

    v0 (actualX, actualY)   ------------   v2 (actualMaxX, actualY)
	     |                                /            |
		 |                          /                  | 
		 |                  /                          |
		 |        /                                    |
	v1 (actualX, actualMaxY)  ----------   v3 (actualMax, actualMaxY)

	Triangle 1: v0, v1, v2
	Triangle 2: v2, v1, v3

Texture Coordinates:

	v0 (xTexCoord, yTexCoord)   -------------------------       v2 (xTexCoord + xTexCoordWidth, yTexCoord)
	     |                                                                          |
	     |                                                                          |
	     |                                                                          |
	     |                                                                          |
	v1 (xTexCoord, yTexCoord - yTexCoordHeight)  ----------   v3 (xTexCoord + xTexCoordWidth, yTexCoord - yTexCoordHeight)

*/

void GUIFont::createDataFromText(std::vector<GUIGlyph>& glyphs, float xPos, float yPos, std::string text, float fontSize, TextAlignment align, float maxWidth)
{
	float cursorX = xPos;
	float cursorY = yPos;
	float textWidth = 0.0f;
	float textHeight = 0.0f;


	for (int i = 0; i < (int) text.length(); i++)
	{
		GUIChar* c = data[(int)text[i]];
		textWidth += c->width * fontSize;
		if ((c->height * fontSize) > textHeight) textHeight = c->height * fontSize;
	}



	if (align == TEXT_ALIGN_LEFT || align == TEXT_ALIGN_MIDDLE || align == TEXT_ALIGN_RIGHT || align == TEXT_ALIGN_BOTTOM || align == TEXT_ALIGN_BOTTOMLEFT)
		cursorY -= textHeight / 2.0f;

	if (align == TEXT_ALIGN_TOP || align == TEXT_ALIGN_MIDDLE || align == TEXT_ALIGN_BOTTOM)
		cursorX -= textWidth / 2.0f;

	if(align == TEXT_ALIGN_BOTTOM || align == TEXT_ALIGN_BOTTOMLEFT)
		cursorY -= textHeight / 2.0f;


	GUIGlyph glyph;

	for (int i = 0; i < (int) text.length(); i++)
	{
		GUIChar* c = data[(int)text[i]];

		float x = cursorX + c->xOffset * fontSize;
		float y = cursorY + c->yOffset * fontSize;
		float maxX = x + (c->width * fontSize);
		float maxY = y + (c->height * fontSize);

		float actualX = (x + 1) / ((float)Application::winWidth * 0.5f) - 1;
		float actualY = -((y + 1) / ((float)Application::winHeight * 0.5f) - 1);
		float actualMaxX = (maxX + 1) / ((float)Application::winWidth * 0.5f) - 1;
		float actualMaxY = -((maxY + 1) / ((float)Application::winHeight * 0.5f) - 1);

		// Tex Coords (Range 0-1 - 0,0 Bottom Left)
		float xTexCoord = c->xPos / 512.0f;
		float yTexCoord = 1 - ((c->yPos) / 512.0f);

		float xTexCoordWidth = c->width / 512.0f;
		float yTexCoordHeight = c->height / 512.0f;


		// Triangle 1
		glyph.position = Vector3(actualX, actualY, 0);
		glyph.texCoord = TexCoord(xTexCoord, yTexCoord);
		glyphs.push_back(glyph);

		glyph.position = Vector3(actualX, actualMaxY, 0);
		glyph.texCoord = TexCoord(xTexCoord, yTexCoord - yTexCoordHeight);
		glyphs.push_back(glyph);


		glyph.position = Vector3(actualMaxX, actualY, 0);
		glyph.texCoord = TexCoord(xTexCoord + xTexCoordWidth, yTexCoord);
		glyphs.push_back(glyph);

		// Triangle 2
		glyph.position = Vector3(actualMaxX, actualY, 0);
		glyph.texCoord = TexCoord(xTexCoord + xTexCoordWidth, yTexCoord);
		glyphs.push_back(glyph);


		glyph.position = Vector3(actualX, actualMaxY, 0);
		glyph.texCoord = TexCoord(xTexCoord, yTexCoord - yTexCoordHeight);
		glyphs.push_back(glyph);

		glyph.position = Vector3(actualMaxX, actualMaxY, 0);
		glyph.texCoord = TexCoord(xTexCoord + xTexCoordWidth, yTexCoord - yTexCoordHeight);
		glyphs.push_back(glyph);


		cursorX += c->xAdvance * fontSize;

		if (actualMaxX * 1024.0f > (xPos + maxWidth))
		{
			cursorX = xPos;
			cursorY += textHeight + 2.0f;
		}

	}
}

void GUIFont::load(const char* path)
{



	std::ifstream handle(path);
	if (!handle.is_open())
	{
		std::cout << "[GUIFont] Failed to load font at " << path << std::endl;
		return;
	}



	std::string line;
	while (std::getline(handle, line))
	{
		if (line.substr(0, 5) == "char ")
		{
			std::vector<std::string> params = Utility::splitLine(line, ' ');
			GUIChar* text = new GUIChar((char)getValue(params[1]), getValue(params[2]), getValue(params[3]), getValue(params[4]),
				getValue(params[5]), getValue(params[6]), getValue(params[7]), getValue(params[8]));
			data[(int) text->id] = text;
		}
	}
	handle.close();
}

GUIChar* GUIFont::getChar(char c)
{
	int ascii = (int)c;
	return data[ascii];
}

unsigned int GUIFont::getVBO()
{
	return VBO;
}

unsigned int GUIFont::getTexture()
{
	return texture;
}

float GUIFont::getValue(std::string input)
{
	auto pos = std::find(input.begin(), input.end(), '=') + 1;
	std::string s(pos, input.end());
	return std::stof(s);
}
