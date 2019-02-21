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

}


GUIManager::~GUIManager()
{
	for (auto const& font : fonts)
		if (font.second != nullptr)
			delete font.second;
}

void GUIManager::InitFBO()
{
	//// Reflection FBO
	//glGenFramebuffers(1, &FBO);
	//glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	//glGenTextures(1, &topdownTexture);
	//glBindTexture(GL_TEXTURE_2D, topdownTexture);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei) 800.0f, (GLsizei) 600.0f, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, topdownTexture, 0);

	//// Reflection RBO
	//glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, (GLsizei) Application::winWidth, (GLsizei) Application::winHeight);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
	//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	//	std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;


	////Unbind
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



GUIManager* GUIManager::getInstance()
{
	if (instance == nullptr)
		instance = new GUIManager();
	return instance;
}





GUIText* GUIManager::renderText(std::string font, float xPos, float yPos, std::string text, float fontSize, Color color, TextAlignment align)
{
	std::vector<GUIGlyph> glyphs;
	fonts[font]->createDataFromText(glyphs, xPos, yPos, text, fontSize, align);
	return new GUIText(glyphs, xPos, yPos, fonts[font]->getTexture(), fontSize, color);
}
