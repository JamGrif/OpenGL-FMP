#include "ModelBasic.h"

ModelBasic::ModelBasic(glm::vec3 position, glm::vec3 rotation)
	:Model(position, rotation)
{

	//setShaderOne
	setShaderTwo("res/assets/shaders/basic-vertex.glsl", "res/assets/shaders/basic-fragment.glsl");
}

ModelBasic::~ModelBasic()
{

}

/// <summary>
/// First draw pass
/// </summary>
void ModelBasic::drawPassOne()
{
	//If no valid model or shader attached
	if (m_modelMesh == nullptr || m_modelShaderPassOne == nullptr)
	{
		return;
	}
}

/// <summary>
/// Second draw pass
/// </summary>
void ModelBasic::drawPassTwo()
{
	//If no valid model or shader attached
	if (m_modelMesh == nullptr || m_modelShaderPassTwo == nullptr)
	{
		return;
	}

	setMatrixValues();

	//Bind shader
	m_modelShaderPassTwo->Bind();

	//Set Vertex values
	m_modelShaderPassTwo->setUniformMatrix4fv("m_matrix", m_mMat);
	m_modelShaderPassTwo->setUniformMatrix4fv("v_matrix", m_vMat);
	m_modelShaderPassTwo->setUniformMatrix4fv("proj_matrix", *EngineStatics::getProjectionMatrix());



	setVBOAttrib(true, false, false, false, false);

	//Draw
	glDrawElements(GL_TRIANGLES, m_modelMesh->getIndices().size(), GL_UNSIGNED_INT, 0);

	m_modelShaderPassTwo->Unbind();
	
}
