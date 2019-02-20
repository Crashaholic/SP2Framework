#include "GUIManager.h"
#include "Manager.h"
#include "GL\glew.h"
#include "Utility.h"
#include "LoadTGA.h"
#include "GUITexture.h"

GUIManager* GUIManager::instance = nullptr;

GUIManager::GUIManager()
{
	fonts["bahnschrift"] = new GUIFont("Fonts//bahnschrift.fnt", "Fonts//bahnschrift.tga");
	fonts[ "consolas"  ] = new GUIFont("Fonts//consolas.fnt"   , "Fonts//consolas.tga"   );
	fonts[  "default"  ] = new GUIFont("Fonts//default.fnt"    , "Fonts//default.tga"    );
	fonts[  "digital"  ] = new GUIFont("Fonts//analogue.fnt"   , "Fonts//analogue.tga"   );
	GUIButton* button1 = new GUIButton(
		Vector3(0, 0), 
		0.0f, 
		Vector3(128.0f, 128.f, 1.f), 
		LoadTGA("Image//rock.tga"), LoadTGA("Image//water.tga"), 
		"name");
	//buttons.push_back(button1);

	InitFBO();
	GUITexture* topdown = new GUITexture
		(
			Vector3(200, 300),
			0,
			Vector3(80.0f, 80.0f, 1),
			topdownTexture
		);
	//renderables.push_back(topdown->getIRender());
	//renderables.push_back(cursor.getGUITexture()->getIRender());


}


GUIManager::~GUIManager()
{
	for (auto const& font : fonts)
		if (font.second != nullptr)
			delete font.second;

	for (int i = 0; i < (int) renderables.size(); i++)
		if(renderables[i] != nullptr)
			delete renderables[i];
}

void GUIManager::InitFBO()
{
	// Reflection FBO
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glGenTextures(1, &topdownTexture);
	glBindTexture(GL_TEXTURE_2D, topdownTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei) 800.0f, (GLsizei) 600.0f, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, topdownTexture, 0);

	// Reflection RBO
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, (GLsizei) Application::winWidth, (GLsizei) Application::winHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;


	//Unbind
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GUIManager::update(double newX, double newY, double winWidth, double winHeight)
{
	cursorUpdate(newX, newY, winWidth, winHeight);
	buttonStateUpdate(cursor.getX(), cursor.getY(), winWidth, winHeight);
}

void GUIManager::cursorUpdate(double newX, double newY, double winWidth, double winHeight)
{
	cursor.updateVars(newX, newY, winWidth, winHeight);
	cursor.updateTexture();
}

void GUIManager::buttonStateUpdate(double newX, double newY, double winWidth, double winHeight)
{
	for (int i = 0; i < (int)buttons.size(); i++) {
		if (buttons[i]->checkStatus(newX, newY, winWidth, winHeight)) {
			if (buttons[i]->getName() == "shop") {
				//...
			}
		}
	}
}

GUIManager* GUIManager::getInstance()
{
	if (instance == nullptr)
		instance = new GUIManager();
	return instance;
}

void GUIManager::renderUI() 
{
	
	for (int i = 0; i < (int)guiText.size(); i++) {
		guiText[i]->render();
		delete guiText[i];
	}

	for (int i = 0; i < (int)buttons.size(); i++) {
		buttons[i]->getIRender()->draw();
	}

	for (int i = 0; i < (int)renderables.size(); i++) {
		renderables[i]->draw();
	}
	guiText.clear();

}

void GUIManager::renderText(std::string font, float xPos, float yPos, std::string text, float fontSize, Color color, TextAlignment align)
{
	std::vector<GUIGlyph> glyphs;
	fonts[font]->createDataFromText(glyphs, xPos, yPos, text, fontSize, align);
	guiText.push_back(new GUIText(glyphs, xPos, yPos, fonts[font]->getTexture(), fontSize, color));

}
