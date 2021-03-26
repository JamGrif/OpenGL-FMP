#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

#include "ErrorChecking.h"


class Shader
{
public:
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	~Shader();

	void Use() const;

	GLuint getProgram() const;

private:
	GLuint m_shaderProgram;
};
