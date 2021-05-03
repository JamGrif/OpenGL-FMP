#include "ModelGeometry.h"

ModelGeometry::ModelGeometry(glm::vec3 position, glm::vec3 rotation)
	:Model(position, rotation)
{
	setShaderTwo("res/shaders/geometry-vertex.glsl", "res/shaders/geometry-geometry.glsl", "res/shaders/geometry-fragment.glsl");
}

ModelGeometry::~ModelGeometry()
{
}

/// <summary>
/// Overriden method from Model base class - Unused in this class
/// </summary>
void ModelGeometry::drawPassOne()
{
}

void ModelGeometry::drawPassTwo()
{
	//If no shader attached
	if (m_modelMesh == nullptr || m_modelShaderPassTwo == nullptr)
	{
		return;
	}
	//std::cout << "geometry" << std::endl;
}

void ModelGeometry::setShaderTwo(const char* vertexPath, const char* geometryPath, const char* fragmentPath)
{
	m_modelShaderPassTwo = ShaderManager::retrieveShader(vertexPath, geometryPath, fragmentPath);
}
