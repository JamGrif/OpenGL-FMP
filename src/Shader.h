#pragma once

#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <GL/glew.h>

#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>




class Shader
{
public:
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void setUniform1i(const std::string& name, int v0);
	void setUniform1f(const std::string& name, float v0);
	void setUniform3f(const std::string& name, const glm::vec3& v0);
	void setUniform4f(const std::string& name, const glm::vec4& v0);
	void setUniformMatrix4fv(const std::string& name, const glm::mat4& v0);

	GLuint getProgram() const;
	const GLchar* getVertexPath() const;
	const GLchar* getFragmentPath() const;

private:
	GLuint m_shaderProgram;

	const GLchar* m_vertexPath;
	const GLchar* m_fragmentPath;

	int getUniformLocation(const std::string& name);

	//Cache for uniforms
	std::unordered_map<std::string, int> m_locationCache;

};



class ShaderManager
{
public:

	static Shader* loadShader(const GLchar* vertexPath, const GLchar* fragmentPath);

private:

	static std::vector<Shader*> loadedShaders;

	ShaderManager();
};