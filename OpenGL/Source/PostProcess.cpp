#include "PostProcess.h"
#include "Application.h"

PostProcess::PostProcess(ShaderProgram* s)
{
	this->shader = s;
}

PostProcess::PostProcess()
{
}

PostProcess::~PostProcess()
{
	glDeleteFramebuffers(1, &this->FBO);
	glDeleteRenderbuffers(1, &this->RBO);
	glDeleteBuffers(1, &this->VBO);
	glDeleteVertexArrays(1, &this->VAO);
}

void PostProcess::init()
{
	glGenFramebuffers(1, &this->FBO);
	glGenRenderbuffers(1, &this->RBO);
	glGenBuffers(1, &this->VBO);
	glGenVertexArrays(1, &this->VAO);
}

unsigned int PostProcess::getTextureID()
{
	return this->texID;
}

void PostProcess::bindFbo()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

void PostProcess::unbindFbo()
{

	float quadVertices[] =
	{
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};


	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Application::winWidth, Application::winHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	glGenTextures(1, &texID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Application::winWidth, Application::winHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texID, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	 
	glDisable(GL_DEPTH_TEST);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	shader->use();
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	
	
	shader->setUniform("screenTexture", 0);
	shader->updateUniforms();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glDeleteTextures(1, &texID);

	glEnable(GL_DEPTH_TEST);

}