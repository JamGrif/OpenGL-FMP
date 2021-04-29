#include "ModelEnvironment.h"

ModelEnvironment::ModelEnvironment(glm::vec3 position, glm::vec3 rotation)
	:Model(position, rotation)
{
	//setShaderOne--------
	setShaderTwo("res/shaders/environmentMapping-vertex.glsl", "res/shaders/environmentMapping-fragment.glsl");
}

ModelEnvironment::~ModelEnvironment()
{
}

void ModelEnvironment::drawPassOne()
{
	//If no valid model or shader attached
	if (m_modelMesh == nullptr || m_modelShaderPassOne == nullptr)
	{
		return;
	}
}

void ModelEnvironment::drawPassTwo()
{
	//If no valid model or shader attached
	if (m_modelMesh == nullptr || m_modelShaderPassTwo == nullptr)
	{
		return;
	}

	//Bind shader
	m_modelShaderPassTwo->Bind();

	//setMatrixValues();

	//Set Vertex values
	m_modelShaderPassTwo->setUniformMatrix4fv("m_matrix", m_mMat);
	m_modelShaderPassTwo->setUniformMatrix4fv("v_matrix", m_vMat);
	m_modelShaderPassTwo->setUniformMatrix4fv("proj_matrix", *EngineStatics::getProjectionMatrix());

	//Draw
	glDrawElements(GL_TRIANGLES, m_modelMesh->getIndices().size(), GL_UNSIGNED_INT, 0);

	m_modelShaderPassTwo->Unbind();
}
