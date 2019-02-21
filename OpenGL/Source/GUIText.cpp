#include "GUIText.h"
#include "GL/glew.h"
#include "Manager.h"


GUIText::GUIText(std::vector<GUIGlyph> glyphs, float xPos, float yPos, unsigned int fontTexture, float fontSize, Color color){
	this->xPos = xPos;
	this->yPos = yPos;
	this->fontSize = fontSize;
	this->color = color;
	this->glyphs = glyphs;
	this->fontTexture = fontTexture;
	//glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
}

GUIText::GUIText()
{
	xPos = yPos = fontSize = 0.0f;
	color = Color(1, 1, 1);
	fontTexture = VBO = 0;
}


GUIText::~GUIText()
{
	glDeleteBuffers(1, &VBO);
	//glDeleteVertexArrays(1, &VAO);
}

void GUIText::render() {

	if (glyphs.size() == 0) return;

	//glBindVertexArray(VAO);

	// Buffer VBO data and layout inputs
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GUIGlyph) * glyphs.size(), &glyphs[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(GUIGlyph), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(GUIGlyph), (void*) sizeof(Vector3));

	Manager* manager = Manager::getInstance();
	ShaderProgram* shader = manager->getShader("text");

	shader->use();

	// Update uniforms and bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fontTexture);
	shader->setUniform("fontSize", fontSize);
	shader->setUniform("textColor", color.r, color.g, color.b);
	shader->updateUniforms();

	// Draw call
	glDrawArrays(GL_TRIANGLES, 0, 6 * glyphs.size());

	// Unbind
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	//glBindVertexArray(0);

}

std::vector<GUIGlyph>* GUIText::getGlyphs() {
	return &glyphs;
}