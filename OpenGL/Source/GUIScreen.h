#ifndef GUI_SCREEN
#define GUI_SCREEN

#include <vector>
#include <map>
#include "GUIText.h"
#include "GUIButton.h"
#include "IRender.h"
#include "Cursor.h"

class GUIScreen
{
public:
	GUIScreen();
	~GUIScreen();
	void Render();
	void Update();
	void addButton(GUIButton* button);
	void addTexture(GUITexture* texture);
private:
	Cursor* cursor;
	std::vector<IRender*> renderables;
	std::vector<GUIText*> text;
	std::vector<GUIButton*> buttons;
};

#endif
