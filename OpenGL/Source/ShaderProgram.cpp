#include "ShaderProgram.h"
#include "GL\glew.h"
#include "shader.hpp"
#include "Utility.h"

ShaderProgram::ShaderProgram(const char* vertexPath, const char* fragPath)
{
	id = LoadShaders(vertexPath, fragPath);
}

ShaderProgram::ShaderProgram()
{
	id = 0;
}


ShaderProgram::~ShaderProgram()
{
}

void ShaderProgram::use()
{
	glUseProgram(id);
}

unsigned int ShaderProgram::getID()
{
	return id;
}

void ShaderProgram::setUniform(std::string name, Mtx44 mat)
{
	matrixes[name] = mat;
}

void ShaderProgram::setUniform(std::string name, Vector3 v)
{
	vectors[name] = v;
}

void ShaderProgram::setUniform(std::string name, float f1, float f2, float f3)
{
	vectors[name] = Vector3(f1, f2, f3);
}



void ShaderProgram::setUniform(std::string name, float f)
{
	floats[name] = f;
}

void ShaderProgram::setUniform(std::string name, int i)
{
	integers[name] = i;
}

void ShaderProgram::updateUniforms()
{
	
	for (auto const& i : integers) {
		unsigned int location = glGetUniformLocation(id, i.first.c_str());
		glUniform1i(location, i.second);
	}

	for (auto const& f : floats) {
		unsigned int location = glGetUniformLocation(id, f.first.c_str());
		glUniform1f(location, f.second);
	}

	for (auto const& m : matrixes) {
		unsigned int location = glGetUniformLocation(id, m.first.c_str());
		glUniformMatrix4fv(location, 1, false, &m.second.a[0]);
	}

	for (auto const& v : vectors) {
		unsigned int location = glGetUniformLocation(id, v.first.c_str());
		glUniform3fv(location, 1, &v.second.x);
	}
	
}