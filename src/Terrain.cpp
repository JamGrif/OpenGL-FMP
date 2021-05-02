#include "Terrain.h"


Terrain::Terrain(glm::vec3 position, glm::vec3 rotation)
	:Model(position, rotation)
{

	setShaderTwo("res/shaders/terrain-vertex.glsl", "res/shaders/terrain-tessCont.glsl", "res/shaders/terrain-tessEval.glsl", "res/shaders/terrain-fragment.glsl");
	//setMesh("res/meshes/plane.obj");

	m_terrainTexture = TextureManager::retrieveTexture("res/textures/grass_diff.png");
}

Terrain::~Terrain()
{
}

void Terrain::drawPassOne()
{
}

void Terrain::drawPassTwo()
{
	//If no valid model or shader attached
	//if (m_modelMesh == nullptr || m_modelShaderPassTwo == nullptr)
	//{
	//	return;
	//}

	m_scale.x = 50;
	m_scale.y = 50;
	m_scale.z = 50;

	m_modelShaderPassTwo->Bind(); 
	glm::mat4 mMVP = *EngineStatics::getProjectionMatrix() * m_vMat * m_mMat;
	m_modelShaderPassTwo->setUniformMatrix4fv("mvp_matrix", mMVP);

	m_terrainTexture->Bind(0);

	glFrontFace(GL_CCW);
	glPatchParameteri(GL_PATCH_VERTICES, 1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_PATCHES, 0, 1);

	glPolygonMode(GL_FRONT, GL_FILL);

	//std::cout << "terrain" << std::endl;


	m_modelShaderPassTwo->Unbind();
	m_terrainTexture->Unbind();

}

void Terrain::setShaderTwo(const char* vertexPath, const char* tessellationControlPath, const char* tessellationEvaluationPath, const char* fragmentPath)
{
	m_modelShaderPassTwo = ShaderManager::loadShader(vertexPath, tessellationControlPath, tessellationEvaluationPath, fragmentPath);
}
