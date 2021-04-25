#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <iostream>

class Texture
{
public:
	Texture(const char* filePath);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;


	const char* getFilePath() const;
	GLuint getTexture() const;
	

private:

	GLuint m_texture;

	const char* m_filePath;

	

	int m_width, m_height, m_BPP;

};

class TextureManager
{
public:

	static Texture* loadTexture(const char* filePath);

private:

	static std::vector<Texture*> loadedTextures;

	TextureManager();
};

