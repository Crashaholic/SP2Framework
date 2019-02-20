#ifndef GUI_SCREEN
#define GUI_SCREEN

#include <vector>
#include <map>
#include "GUIText.h"
#include "GUIButton.h"
#include "Texture.h"
#include "IRender.h"
#include "Cursor.h"

class GUIScreen
{
public:
	GUIScreen();
	~GUIScreen();
	void render();
	
	void addButton(GUIButton* button);

private:
	std::vector<IRender*> renderables;
	std::vector<GUIText*> text;
	std::vector<GUIButton*> buttons;
};

#endif
