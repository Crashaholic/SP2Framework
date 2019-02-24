#ifndef POST_PROCESS
#define POST_PROCESS

#include <GL/glew.h>
#include "ShaderProgram.h"

class PostProcess
{
private:
	unsigned int texID;
	unsigned int FBO;
	unsigned int RBO;
	unsigned int VBO;
	unsigned int VAO;

	ShaderProgram* shader;

public:
	PostProcess(ShaderProgram* shader);
	PostProcess();
	~PostProcess();
	void init();
	unsigned int getTextureID();
	void bindFbo();
	void unbindFbo();

};

#endif
