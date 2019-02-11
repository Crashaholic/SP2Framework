#include "GUIManager.h"
#include "Manager.h"
#include "GL\glew.h"
#include "Utility.h"
#include "LoadTGA.h"

GUIManager* GUIManager::instance = nullptr;



GUIManager::GUIManager()
{
	fonts["game"] = new GUIFont("Fonts//game2.fnt", "Fonts//game2.tga");


	//InitFBO();

	//textures.push_back(new Texture(Vector3(0.7f, 0.7f), 0, Vector3(0.25f, 0.25f), topdownTexture, -1));
	//textures.push_back(new Texture(Vector3(0.7f, 0.7f), 90, Vector3(0.025f, 0.025f), LoadTGA("Image//arrow.tga"), 1));
}


GUIManager::~GUIManager()
{


	for (auto const& font : fonts)
		if (font.second != nullptr)
			delete font.second;

	for (int i = 0; i < (int) textures.size(); i++)
		if(textures[i] != nullptr)
			delete textures[i];
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
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, (GLsizei) 800.0f, (GLsizei) 600.0f);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
	//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	//	std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;


	//// Unbind
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Texture* GUIManager::getTexture(int pos) {
	return textures.at(pos);
}




GUIManager* GUIManager::getInstance()
{
	if (instance == nullptr)
		instance = new GUIManager();
	return instance;
}


void GUIManager::renderUI() {
	

	/*glBindVertexArray(VAO);

	float quadVertices[] = {
		-1.0f,  1.0f,
		-1.0f, -1.0f,
		1.0f,  1.0f,
		1.0f, -1.0f,

	};

	glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(float) * 2, (void*)0);


	ShaderProgram* shader = Manager::getInstance()->getShader("overlay");
	shader->use();

	glDisable(GL_DEPTH_TEST);
	for (int i = 0; i < (int)textures.size(); i++)
	{
		Texture t = *textures.at(i);
		Mtx44 mat = t.getTransformationMatrix();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, t.getID());
		shader->setUniform("dontFlip", t.getFlip());
		shader->setUniform("transformationMatrix", mat.a);
		shader->updateUniforms();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	}
	glEnable(GL_DEPTH_TEST);
	glDisableVertexAttribArray(0);*/

	for (int i = 0; i < (int)guiText.size(); i++) {
		guiText[i]->render();
		delete guiText[i];
	}		
	guiText.clear();

}

void GUIManager::renderText(std::string font, float xPos, float yPos, std::string text, float fontSize, Color color, TextAlignment align)
{
	std::vector<GUIGlyph> glyphs;
	fonts[font]->createDataFromText(glyphs, xPos, yPos, text, fontSize, align);
	guiText.push_back(new GUIText(glyphs, xPos, yPos, fonts[font]->getTexture(), fontSize, color));
}