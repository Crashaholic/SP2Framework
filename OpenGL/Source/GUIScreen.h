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
	~GUIScreen();
	void render();
	void pushAsRender(IRender* r);
	void pushAsText(GUIText* t);
	void pushAsButton(GUIButton* b);
private:
	std::vector<IRender*> iRender;
	std::vector<GUIText*> guiText;
	std::vector<GUIButton*> guiButton;
};

#endif
