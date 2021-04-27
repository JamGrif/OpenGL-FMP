#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <iostream>

class Texture
{
public:
	Texture();
	~Texture();

	bool loadTexture(const char* filePath);

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;


	const char* getFilePath() const;
	GLuint getTexture() const;
	

private:

	GLuint m_texture;

	const char* m_filePath;

	int m_width, m_height, m_BPP;

};

class CubeMap
{
public:


private:

};

class TextureManager
{
public:

	static Texture* retrieveTexture(const char* filePath);
	static Texture* loadCubeMap(const char* filePath);

private:

	static std::vector<Texture*> loadedTextures;

	TextureManager();
};

