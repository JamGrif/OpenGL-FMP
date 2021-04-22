#include "ModelBasic.h"

ModelBasic::ModelBasic(glm::vec3 position, glm::vec3 rotation)
	:Model(position, rotation)
{

	//setShaderOne
	setShaderTwo("res/shaders/basic-vertex.glsl", "res/shaders/basic-fragment.glsl");
}

ModelBasic::~ModelBasic()
{

}

void ModelBasic::drawPassOne()
{
	//If no valid model or shader attached
	if (m_modelMesh == nullptr || m_modelShaderPassOne == nullptr)
	{
		return;
	}
}

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



	setVBOAttrib(true, false, false);

	//Draw
	glDrawArrays(GL_TRIANGLES, 0, m_modelMesh->getNumVertices());

	m_modelShaderPassTwo->Unbind();
	
}
