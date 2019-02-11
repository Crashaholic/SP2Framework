#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <map>
#include <string>
#include "Mtx44.h"

class ShaderProgram
{
public:
	ShaderProgram(const char* vertexPath, const char* fragPath);
	ShaderProgram();
	~ShaderProgram();
	void use();
	void setUniform(std::string name, Mtx44 mat);
	void setUniform(std::string name, Vector3 v);
	void setUniform(std::string name, float f1, float f2, float f3);
	void setUniform(std::string name, float f);
	void setUniform(std::string name, int i);
	void updateUniforms();
	unsigned int getID();
private:
	std::map<std::string, float> floats;
	std::map<std::string, Vector3> vectors;
	std::map<std::string, Mtx44> matrixes;
	std::map<std::string, int> integers;
	unsigned int id;
};

#endif