#include "Overlay.h"
#include "GL/glew.h"
#include "Manager.h"
#include "shader.hpp"




Overlay::Overlay()
{
	glGenBuffers(1, &VBO);

}


Overlay::~Overlay()
{
	for (int i = 0; i < (int)textures.size(); i++)
		delete textures[i];
}
//
//void Overlay::addTexture(Vector3 position, Vector3 scale, const char *filePath, int dontFlip)
//{
//	Texture* texture = new Texture(position, scale, filePath, dontFlip);
//	textures.push_back(texture);
//}
//
//void Overlay::addTexture(Vector3 position, Vector3 scale, unsigned int id, int dontFlip)
//{
//	Texture* texture = new Texture(position, scale, id, dontFlip);
//	textures.push_back(texture);
//}


//void Overlay::render()
//{
//
//
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
//
//	glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(float) * 2, (void*)0);
//	glEnableVertexAttribArray(0);
//
//	ShaderProgram* shader = Manager::getInstance()->getShader("overlay");
//	shader->use();
//
//	for (int i = 0; i < (int)textures.size(); i++)
//	{
//		Texture t = *textures.at(i);
//		Mtx44 mat = t.getTransformationMatrix();
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, t.getID());
//		shader->setUniform("dontFlip", t.getFlip());
//		shader->setUniform("transformationMatrix", mat.a);
//		shader->updateUniforms();
//		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//
//	}
//
//	glDisableVertexAttribArray(0);
//
//
//}

//void Overlay::renderTexture(int pos) {
//
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
//
//	glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(float) * 2, (void*)0);
//	glEnableVertexAttribArray(0);
//
//	ShaderProgram* shader = Manager::getInstance()->getShader("overlay");
//	shader->use();
//
//	Texture t = *textures.at(pos);
//	Mtx44 mat = t.getTransformationMatrix();
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, t.getID());
//	shader->setUniform("dontFlip", t.getFlip());
//	shader->setUniform("transformationMatrix", mat.a);
//	shader->updateUniforms();
//	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//
//	
//
//	glDisableVertexAttribArray(0);
//}