#include "Texture.h"

std::vector<Texture*> TextureManager::loadedTextures;

Texture::Texture(const char* filePath)
{
	m_filePath = filePath;
	GLuint textureID;
	textureID = SOIL_load_OGL_texture(filePath, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y); //Generates texture object

	if (textureID == 0)
	{
		std::cout << "Could not find requested texture! " << filePath << std::endl;
		return;
	}

	glBindTexture(GL_TEXTURE_2D, textureID); //Activate texture

	//Mipmap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); //Apply Mipmapping
	glGenerateMipmap(GL_TEXTURE_2D);

	//Anisotropic filtering
	if (glewIsSupported("GL_EXT_texture_filter_anisotropic")) //Ensure supported
	{
		GLfloat anisoSetting = 0.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisoSetting); //Set anisoSetting to maximum sampling support
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisoSetting); //Apply Anisotropic Filtering
	}

	//Specify what happens to texture coords outside 0-1 range
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	m_filePath = filePath;
	m_texture = textureID;
}

Texture::~Texture()
{
}

void Texture::loadTexture(const char* filePath)
{
	
}

const char* Texture::getFilePath() const
{
	return m_filePath;
}

GLuint Texture::getTexture() const
{
	return m_texture;
}

Texture* TextureManager::loadTexture(const char* filePath)
{
	//Check if texture is already loaded
	for (Texture* t : loadedTextures)
	{
		if (t->getFilePath() == filePath)
		{
			//std::cout << "TEXTUREMANAGER->" << filePath << " already exists, returning loaded texture" << std::endl;
			return t;
		}
	}

	//Otherwise, create new texture and add it to vector
	std::cout << "TEXTUREMANAGER->" << filePath << " is being loaded" << std::endl;

	loadedTextures.push_back(new Texture(filePath));
	return loadedTextures.back();
}
