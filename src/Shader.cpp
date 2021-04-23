#include "Shader.h"

std::vector<Shader*> ShaderManager::loadedShaders;

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
	:m_vertexPath(vertexPath), m_fragmentPath(fragmentPath), m_shaderProgram(0)
{
	//std::cout << "Shader Initialized" << std::endl;

	//Retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	//Ensure ifstream objects can throw exceptions
	vShaderFile.exceptions(std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::badbit);

	try
	{
		//Open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;

		//Read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		//Close File handlers
		vShaderFile.close();
		fShaderFile.close();

		//Convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();

	//Compile our shader program
	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];

	//Vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

	//Check if shader has successfully compiled
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Fragment
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

	//Check if shader has successfully compiled
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Shader program
	m_shaderProgram = glCreateProgram();
	glAttachShader(m_shaderProgram, vertex);
	glAttachShader(m_shaderProgram, fragment);
	glLinkProgram(m_shaderProgram);

	//Check for linking errors
	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(m_shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	//Delete the shaders as they're linked into our program now and no longer neccessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::~Shader()
{
	std::cout << "Shader Destroyed" << std::endl;

	glDeleteProgram(m_shaderProgram);
}

void Shader::Bind() const
{
	glUseProgram(m_shaderProgram);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

void Shader::setUniform1i(const std::string& name, int v0) 
{
	glUniform1i(getUniformLocation(name), v0);
}

void Shader::setUniform1f(const std::string& name, float v0) 
{
	glUniform1f(getUniformLocation(name), v0);
}

void Shader::setUniform3f(const std::string& name, const glm::vec3& v0) 
{
	glUniform3f(getUniformLocation(name), v0.x, v0.y, v0.z);
}

void Shader::setUniform4f(const std::string& name, const glm::vec4& v0) 
{
	glUniform4f(getUniformLocation(name), v0.x, v0.y, v0.z, v0.w);
}

void Shader::setUniformMatrix4fv(const std::string& name, const glm::mat4& v0) 
{
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(v0));
}

void Shader::setUniformMatrix3fv(const std::string& name, const glm::mat3& v0)
{
	glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(v0));
}


GLuint Shader::getProgram() const
{
	return m_shaderProgram;
}

const GLchar* Shader::getVertexPath() const
{
	return m_vertexPath;
}

const GLchar* Shader::getFragmentPath() const
{
	return m_fragmentPath;
}

int Shader::getUniformLocation(const std::string& name) 
{
	if (m_locationCache.find(name) != m_locationCache.end())
	{
		return m_locationCache[name];
	}

	int location = glGetUniformLocation(m_shaderProgram, name.c_str());
	if (location == -1)
	{
		std::cout << "SHADER->Uniform location " << name << " does not exist!" << std::endl;
	}
	m_locationCache[name] = location;
	
	return location;
}

Shader* ShaderManager::loadShader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	//Check if shader program is already loaded
	for (Shader* s : loadedShaders)
	{
		if (s->getVertexPath() == vertexPath && s->getFragmentPath() == fragmentPath)
		{
			//std::cout << "SHADERMANAGER->" << vertexPath << " + " << fragmentPath << " program already exists, returning loaded shader program" << std::endl;
			return s;
		}
	}

	//Otherwise, create new texture and add it to vector
	std::cout << "SHADERMANAGER->" << vertexPath << " + " << fragmentPath << " program is being created" << std::endl;

	loadedShaders.push_back(new Shader(vertexPath, fragmentPath));
	return loadedShaders.back();
}
