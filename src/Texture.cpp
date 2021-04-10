#include "Texture.h"

#include "stb_image.h"

std::vector<Texture*> TextureManager::loadedTextures;

Texture::Texture(const char* filePath)
	:m_filePath(filePath), m_localBuffer(nullptr), m_width(0), m_height(0), m_BPP(0)
{

	stbi_set_flip_vertically_on_load(1); //Flips texture on Y-Axis
	m_localBuffer = stbi_load(filePath, &m_width, &m_height, &m_BPP, 4);
	
	//Check if file loaded successfully
	if (stbi_failure_reason() == "can't fopen")
	{
		std::cout << "TEXTURE->" << m_filePath << " failed to load, loading default texture" << std::endl;
		m_localBuffer = stbi_load("res/textures/missingtexture.png", &m_width, &m_height, &m_BPP, 4);
		m_filePath = "res/textures/missingtexture.png";
	}

	//Generate texture buffer
	glGenTextures(1, &m_texture);

	glBindTexture(GL_TEXTURE_2D, m_texture);
	
	//Specify what happens if texture is renderered on a different sized surface
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Specify what happens to texCoords outside 0-1 range
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	//Anisotropic filtering
	if (glewIsSupported("GL_EXT_texture_filter_anisotropic")) //Ensure supported
	{
		GLfloat anisoSetting = 0.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisoSetting); //Set anisoSetting to maximum sampling support
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisoSetting); //Apply Anisotropic Filtering
	}

	//Define the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer); 
	glGenerateMipmap(GL_TEXTURE_2D);

	//Unbind
	glBindTexture(GL_TEXTURE_2D, 0);

	if (m_localBuffer)
	{
		stbi_image_free(m_localBuffer);
	}

}

Texture::~Texture()
{
	glDeleteTextures(1, &m_texture);
	std::cout << "deleted texture" << std::endl;
}

void Texture::Bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_texture);
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
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
