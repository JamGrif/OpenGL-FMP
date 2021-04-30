#include "Texture.h"

#include "stb_image.h"

std::vector<Texture*> TextureManager::loadedTextures;
std::vector<CubeMap*> TextureManager::loadedCubemaps;

Texture::Texture()
	:m_texture(0), m_width(0), m_height(0), m_BPP(0)
{
	
}

Texture::~Texture()
{
	std::cout << "deleted texture" << std::endl;
	glDeleteTextures(1, &m_texture);
}

/// <summary>
/// Loads a texture from the specified filepath and sets its parameters
/// </summary>
/// <param name="filePath"></param>
/// <returns></returns>
bool Texture::loadTexture(const char* filePath)
{
	m_filePath = filePath;

	unsigned char* localBuffer;

	stbi_set_flip_vertically_on_load(1); //Flips texture on Y-Axis
	localBuffer = stbi_load(filePath, &m_width, &m_height, &m_BPP, 4);

	//Check if file loaded successfully
	if (stbi_failure_reason() == "can't fopen")
	{
		std::cout << "TEXTURE->" << m_filePath << " failed to load, loading default texture" << std::endl;
		localBuffer = stbi_load("res/textures/missingtexture.png", &m_width, &m_height, &m_BPP, 4);
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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);
	glGenerateMipmap(GL_TEXTURE_2D);

	//Unbind
	glBindTexture(GL_TEXTURE_2D, 0);

	if (localBuffer)
	{
		stbi_image_free(localBuffer);
	}

	return true;
}

/// <summary>
/// Binds the texture to the rendering pipeline and to specified texture slot
/// </summary>
/// <param name="slot">Texture slot to bind to</param>
void Texture::Bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_texture);
}

/// <summary>
/// Unbinds texture from the pipeline
/// </summary>
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

/// <summary>
/// Loads the specified texture, if texture already exists then it returns a pointer to it instead of reloading the same texture
/// </summary>
/// <param name="filePath"></param>
/// <returns>Pointer to the created texture</returns>
Texture* TextureManager::retrieveTexture(const char* filePath)
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

	Texture* t = new Texture;
	t->loadTexture(filePath);
	loadedTextures.push_back(t);
	return loadedTextures.back();
}

/// <summary>
/// Loads the specified cubemap texture, if texture already exists then it returns a pointer to it instead of reloading the same cubemap texture
/// </summary>
/// <returns>Pointer to the created texture</returns>
CubeMap* TextureManager::retrieveCubeMap()
{
	//If a loaded cubemap exists, return a pointer to it
	if (loadedCubemaps.size() > 0)
	{
		return loadedCubemaps.back();
	}

	//Otherwise create a new cubemap
	CubeMap* t = new CubeMap;
	t->loadCubeMap();
	loadedCubemaps.push_back(t);
	return loadedCubemaps.back();
}

void TextureManager::clearTextures()
{
	for (Texture* t : loadedTextures)
	{
		delete t;
		t = nullptr;
	}
	loadedTextures.clear();
}

void TextureManager::clearCubemaps()
{
	for (CubeMap* cm : loadedCubemaps)
	{
		delete cm;
		cm = nullptr;
	}
	loadedCubemaps.clear();
}

CubeMap::CubeMap()
{
}

CubeMap::~CubeMap()
{
}

/// <summary>
/// Binds the cubemap to the rendering pipeline and to specified cubemap texture slot
/// </summary>
/// <param name="slot">Cubemap texture slot to bind to</param>
void CubeMap::Bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
}

/// <summary>
/// Unbinds cubemap texture from the pipeline
/// </summary>
void CubeMap::Unbind() const
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

/// <summary>
/// Loads a cubemap texture from the specified filepath and sets its parameters
/// </summary>
/// <returns></returns>
bool CubeMap::loadCubeMap()
{
	//m_filePath = filePath;

	unsigned char* localBuffer;

	stbi_set_flip_vertically_on_load(0); //Flips texture on Y-Axis

	//Generate texture buffer
	glGenTextures(1, &m_texture);

	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);

	for (unsigned int i = 0; i < skyFaces.size(); i++)
	{
		localBuffer = stbi_load(skyFaces[i], &m_width, &m_height, &m_BPP, 0);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);
		stbi_image_free(localBuffer);
	}

	//Specify what happens if texture is renderered on a different sized surface
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Specify what happens to texCoords outside 0-1 range
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	

	//Unbind
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return true;
}
