#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <SOIL2.h>

#include <vector>
#include <iostream>

class Texture
{
public:
	Texture(const char* filePath);
	~Texture();

	void loadTexture(const char* filePath);

	const char* getFilePath() const;
	GLuint getTexture() const;
	

private:

	GLuint m_texture;
	const char* m_filePath;

};



class TextureManager
{
public:

	static Texture* loadTexture(const char* filePath);

private:

	static std::vector<Texture*> loadedTextures;

	TextureManager();
};

