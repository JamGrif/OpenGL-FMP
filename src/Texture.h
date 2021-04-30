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

	virtual void Bind(unsigned int slot = 0) const;
	virtual void Unbind() const;


	const char* getFilePath() const;
	GLuint getTexture() const;
	

protected:

	GLuint		m_texture;

	const char* m_filePath;

	int			m_width, m_height, m_BPP;

};

class CubeMap :
	public Texture
	
{
public:
	CubeMap();
	~CubeMap();

	void Bind(unsigned int slot = 0) const override;
	void Unbind() const override;

	bool loadCubeMap();

private:

	//Overwrite these files to import custom skybox
	std::vector<const char*> skyFaces = 
	{ 
		"res/textures/sky/right.png" ,
		"res/textures/sky/left.png",
		"res/textures/sky/top.png",
		"res/textures/sky/bottom.png",
		"res/textures/sky/front.png",
		"res/textures/sky/back.png"
	};

};

class TextureManager
{
public:

	static Texture* retrieveTexture(const char* filePath);
	static CubeMap* retrieveCubeMap();

	static void clearTextures();
	static void clearCubemaps();

private:

	static std::vector<Texture*> loadedTextures;
	static std::vector<CubeMap*> loadedCubemaps;

	TextureManager();
};

