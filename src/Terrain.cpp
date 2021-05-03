#include "Terrain.h"


Terrain::Terrain(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, float elevation)
	:Model(position, rotation), m_elevation(elevation), m_minElevation(-8), m_maxElevation(-3)
{
	//m_position.y = 22.0f;
	m_scale.x = scale.x;
	m_scale.y = scale.y;
	m_scale.z = scale.z;

	setShaderTwo("res/shaders/terrain-vertex.glsl", "res/shaders/terrain-tessCont.glsl", "res/shaders/terrain-tessEval.glsl", "res/shaders/terrain-fragment.glsl");

	m_terrainTexture = TextureManager::retrieveTexture("res/textures/terrain_diff.png");
	m_terrainHeight = TextureManager::retrieveTexture("res/textures/terrain_height.png");
}

Terrain::~Terrain()
{
}

void Terrain::drawPassOne()
{
}

void Terrain::drawPassTwo()
{
	//If no shader attached
	if (m_modelShaderPassTwo == nullptr)
	{
		return;
	}

	m_modelShaderPassTwo->Bind(); 
	glm::mat4 mMVP = *EngineStatics::getProjectionMatrix() * m_vMat * m_mMat;
	m_modelShaderPassTwo->setUniformMatrix4fv("mvp_matrix", mMVP);

	m_modelShaderPassTwo->setUniform1f("elevation", m_elevation);

	m_modelShaderPassTwo->setUniform1i("texture_color", 0);
	m_modelShaderPassTwo->setUniform1i("texture_height", 1);

	m_terrainTexture->Bind(0);
	m_terrainHeight->Bind(1);

	
	glPatchParameteri(GL_PATCH_VERTICES, 16);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_PATCHES, 0, 16);

	glPolygonMode(GL_FRONT, GL_FILL);

	m_modelShaderPassTwo->Unbind();
	m_terrainTexture->Unbind();

}

void Terrain::setShaderTwo(const char* vertexPath, const char* tessellationControlPath, const char* tessellationEvaluationPath, const char* fragmentPath)
{
	m_modelShaderPassTwo = ShaderManager::loadShader(vertexPath, tessellationControlPath, tessellationEvaluationPath, fragmentPath);
}

void Terrain::alterElevation(float change)
{
		m_elevation += change;

		if (m_elevation < m_minElevation)
				m_elevation = m_minElevation;

		if (m_elevation > m_maxElevation)
				m_elevation = m_maxElevation;

	
}
