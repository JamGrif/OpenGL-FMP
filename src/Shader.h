#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <GL/glew.h>

#include "ErrorChecking.h"


class Shader
{
public:
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	~Shader();

	void Use() const;

	GLuint getProgram() const;

	const GLchar* getVertexPath();
	const GLchar* getFragmentPath();

private:
	GLuint m_shaderProgram;

	const GLchar* m_vertexPath;
	const GLchar* m_fragmentPath;
};



class ShaderManager
{
public:

	static Shader* loadShader(const GLchar* vertexPath, const GLchar* fragmentPath);

private:

	static std::vector<Shader*> loadedShaders;

	ShaderManager();
};