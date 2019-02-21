#ifndef GUI_SCREEN
#define GUI_SCREEN

#include <vector>
#include <map>
#include "GUIText.h"
#include "GUIButton.h"
#include "IRender.h"
#include "Cursor.h"
#include <string>

class GUIScreen
{
public:
	GUIScreen(std::string name);
	GUIScreen();
	~GUIScreen();

	void Render();
	void Update();

	void addButton(GUIButton* button);
	void addTexture(GUITexture* texture);
	void addText(GUIText* text);

private:
	std::string name;
	Cursor* cursor;
	std::vector<IRender*> renderables;
	std::vector<GUIText*> texts;
	std::vector<GUIButton*> buttons;
};

#endif
