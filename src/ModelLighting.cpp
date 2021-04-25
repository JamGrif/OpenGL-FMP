#include "ModelLighting.h"

ModelLighting::ModelLighting(glm::vec3 position, glm::vec3 rotation)
	:Model(position, rotation), m_modelDiffuseTexture(nullptr), m_modelSpecularTexture(nullptr), m_modelEmissionTexture(nullptr), m_modelNormalTexture(nullptr)
{

	m_localLightManager = EngineStatics::getLightManager();

	setShaderOne("res/assets/shaders/lightingPassOne-vertex.glsl", "res/assets/shaders/lightingPassOne-fragment.glsl");
	setShaderTwo("res/assets/shaders/lightingPassTwo-vertex.glsl", "res/assets/shaders/lightingPassTwo-fragment.glsl");
}

ModelLighting::~ModelLighting()
{
	if (m_modelDiffuseTexture != nullptr)
	{
		m_modelDiffuseTexture = nullptr;
	}

	if (m_modelSpecularTexture != nullptr)
	{
		m_modelSpecularTexture = nullptr;
	}

	if (m_modelEmissionTexture != nullptr)
	{
		m_modelEmissionTexture = nullptr;
	}

	if (m_modelNormalTexture != nullptr)
	{
		m_modelNormalTexture = nullptr;
	}
}

/// <summary>
/// First draw pass
/// </summary>
void ModelLighting::drawPassOne()
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
void ModelLighting::drawPassTwo()
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

	/*
		Set Fragment values
	*/

	
	//Ensure a directional light exists
	if (m_localLightManager->getCurrentDirectionalLights() > 0)
	{
		m_modelShaderPassTwo->setUniform3f("dLight.ambient", m_localLightManager->getDirectionalLight(0)->Ambient);
		m_modelShaderPassTwo->setUniform3f("dLight.diffuse", m_localLightManager->getDirectionalLight(0)->Diffuse);
		m_modelShaderPassTwo->setUniform3f("dLight.specular", m_localLightManager->getDirectionalLight(0)->Specular);
		m_modelShaderPassTwo->setUniform3f("dLight.direction", m_localLightManager->getDirectionalLight(0)->Direction);
	}

	//Ensure a point light exists
	if (m_localLightManager->getCurrentPointLights() > 0)
	{
		//Point lights
		if (m_localLightManager->getCurrentPointLights() >= 1)
		{
			m_modelShaderPassTwo->setUniform3f("pLight[0].ambient", m_localLightManager->getPointLight(0)->Ambient);
			m_modelShaderPassTwo->setUniform3f("pLight[0].diffuse", m_localLightManager->getPointLight(0)->Diffuse);
			m_modelShaderPassTwo->setUniform3f("pLight[0].specular", m_localLightManager->getPointLight(0)->Specular);
			m_modelShaderPassTwo->setUniform3f("pLight[0].position", m_localLightManager->getPointLight(0)->Position);
			m_modelShaderPassTwo->setUniform1f("pLight[0].constant", m_localLightManager->getPointLight(0)->Constant);
			m_modelShaderPassTwo->setUniform1f("pLight[0].linear", m_localLightManager->getPointLight(0)->Linear);
			m_modelShaderPassTwo->setUniform1f("pLight[0].quadratic", m_localLightManager->getPointLight(0)->Quadratic);
		}

		if (m_localLightManager->getCurrentPointLights() >= 2)
		{
			m_modelShaderPassTwo->setUniform3f("pLight[1].ambient", m_localLightManager->getPointLight(1)->Ambient);
			m_modelShaderPassTwo->setUniform3f("pLight[1].diffuse", m_localLightManager->getPointLight(1)->Diffuse);
			m_modelShaderPassTwo->setUniform3f("pLight[1].specular", m_localLightManager->getPointLight(1)->Specular);
			m_modelShaderPassTwo->setUniform3f("pLight[1].position", m_localLightManager->getPointLight(1)->Position);
			m_modelShaderPassTwo->setUniform1f("pLight[1].constant", m_localLightManager->getPointLight(1)->Constant);
			m_modelShaderPassTwo->setUniform1f("pLight[1].linear", m_localLightManager->getPointLight(1)->Linear);
			m_modelShaderPassTwo->setUniform1f("pLight[1].quadratic", m_localLightManager->getPointLight(1)->Quadratic);
		}

		if (m_localLightManager->getCurrentPointLights() >= 3)
		{
			m_modelShaderPassTwo->setUniform3f("pLight[2].ambient", m_localLightManager->getPointLight(2)->Ambient);
			m_modelShaderPassTwo->setUniform3f("pLight[2].diffuse", m_localLightManager->getPointLight(2)->Diffuse);
			m_modelShaderPassTwo->setUniform3f("pLight[2].specular", m_localLightManager->getPointLight(2)->Specular);
			m_modelShaderPassTwo->setUniform3f("pLight[2].position", m_localLightManager->getPointLight(2)->Position);
			m_modelShaderPassTwo->setUniform1f("pLight[2].constant", m_localLightManager->getPointLight(2)->Constant);
			m_modelShaderPassTwo->setUniform1f("pLight[2].linear", m_localLightManager->getPointLight(2)->Linear);
			m_modelShaderPassTwo->setUniform1f("pLight[2].quadratic", m_localLightManager->getPointLight(2)->Quadratic);
		}

		if (m_localLightManager->getCurrentPointLights() >= 4)
		{
			m_modelShaderPassTwo->setUniform3f("pLight[3].ambient", m_localLightManager->getPointLight(3)->Ambient);
			m_modelShaderPassTwo->setUniform3f("pLight[3].diffuse", m_localLightManager->getPointLight(3)->Diffuse);
			m_modelShaderPassTwo->setUniform3f("pLight[3].specular", m_localLightManager->getPointLight(3)->Specular);
			m_modelShaderPassTwo->setUniform3f("pLight[3].position", m_localLightManager->getPointLight(3)->Position);
			m_modelShaderPassTwo->setUniform1f("pLight[3].constant", m_localLightManager->getPointLight(3)->Constant);
			m_modelShaderPassTwo->setUniform1f("pLight[3].linear", m_localLightManager->getPointLight(3)->Linear);
			m_modelShaderPassTwo->setUniform1f("pLight[3].quadratic", m_localLightManager->getPointLight(3)->Quadratic);
		}
	}

	//Ensure a spot light exists
	if (m_localLightManager->getCurrentSpotLights() > 0)
	{
		m_modelShaderPassTwo->setUniform3f("sLight.ambient", m_localLightManager->getSpotLight(0)->Ambient);
		m_modelShaderPassTwo->setUniform3f("sLight.diffuse", m_localLightManager->getSpotLight(0)->Diffuse);
		m_modelShaderPassTwo->setUniform3f("sLight.specular", m_localLightManager->getSpotLight(0)->Specular);
		m_modelShaderPassTwo->setUniform3f("sLight.position", EngineStatics::getCamera()->getPosition());
		m_modelShaderPassTwo->setUniform3f("sLight.direction", EngineStatics::getCamera()->getFront());
		m_modelShaderPassTwo->setUniform1f("sLight.cutOff", glm::cos(glm::radians(m_localLightManager->getSpotLight(0)->cutOff)));
		m_modelShaderPassTwo->setUniform1f("sLight.outerCutOff", glm::cos(glm::radians(m_localLightManager->getSpotLight(0)->outerCutOff)));
		m_modelShaderPassTwo->setUniform1f("sLight.constant", m_localLightManager->getSpotLight(0)->Constant);
		m_modelShaderPassTwo->setUniform1f("sLight.linear", m_localLightManager->getSpotLight(0)->Linear);
		m_modelShaderPassTwo->setUniform1f("sLight.quadratic", m_localLightManager->getSpotLight(0)->Quadratic);
	}


	//Material properties
	m_modelShaderPassTwo->setUniform1i("material.diffuse", 0);
	m_modelShaderPassTwo->setUniform1i("material.specular", 1);
	m_modelShaderPassTwo->setUniform1i("material.emission", 2);
	//m_modelShaderPassTwo->setUniform1i("material.normal", 3);
	m_modelShaderPassTwo->setUniform1f("material.shininess", 32.0f);

	//Camera Position
	m_modelShaderPassTwo->setUniform3f("viewPos", EngineStatics::getCamera()->getPosition());


	//Bind textures to pipeline
	if (m_modelDiffuseTexture != nullptr)
	{
		m_modelDiffuseTexture->Bind(0);
	}

	if (m_modelSpecularTexture != nullptr)
	{
		m_modelSpecularTexture->Bind(1);
	}

	if (m_modelEmissionTexture != nullptr)
	{
		m_modelEmissionTexture->Bind(2);
	}

	if (m_modelNormalTexture != nullptr)
	{
		m_modelNormalTexture->Bind(3);
	}

	/*
		Bind VBOs and vertex attributes
	*/

	if (m_modelNormalTexture != nullptr)
	{
		setVBOAttrib(true, true, true, true, true);
	}
	else
	{
		setVBOAttrib(true, true, true, false, false);
	}
	
	//Draw
	glDrawElements(GL_TRIANGLES, m_modelMesh->getIndices().size(), GL_UNSIGNED_INT, 0);

	if (m_modelDiffuseTexture != nullptr)
	{
		m_modelDiffuseTexture->Unbind();
	}

	if (m_modelSpecularTexture != nullptr)
	{
		m_modelSpecularTexture->Unbind();
	}

	if (m_modelEmissionTexture != nullptr)
	{
		m_modelEmissionTexture->Unbind();
	}

	if (m_modelNormalTexture != nullptr)
	{
		m_modelNormalTexture->Unbind();
	}

	if (m_modelShaderPassTwo != nullptr)
	{
		m_modelShaderPassTwo->Unbind();
	}

}

/// <summary>
/// Assigns specified texture to the model to be used for diffuse
/// </summary>
/// <param name="texturePath"></param>
void ModelLighting::setDiffuseTexture(const char* texturePath)
{
	m_modelDiffuseTexture = TextureManager::loadTexture(texturePath);
}

/// <summary>
/// Assigns specified texture to the model to be used for specular
/// </summary>
/// <param name="texturePath"></param>
void ModelLighting::setSpecularTexture(const char* texturePath)
{
	m_modelSpecularTexture = TextureManager::loadTexture(texturePath);
}

/// <summary>
/// Assigns specified texture to the model to be used for emission
/// </summary>
/// <param name="texturePath"></param>
void ModelLighting::setEmissionTexture(const char* texturePath)
{
	m_modelEmissionTexture = TextureManager::loadTexture(texturePath);
}

/// <summary>
/// Assigns specified texture to the model to be used for normal
/// </summary>
/// <param name="texturePath"></param>
void ModelLighting::setNormalTexture(const char* texturePath)
{
	//m_modelNormalTexture = TextureManager::loadTexture(texturePath);
	//
	//if (m_modelNormalTexture == nullptr)
	//	return;
	//
	////Create normal 
	//for (int i = 0; i < m_modelMesh->getVertices().size(); i += 3)
	//{
	//	//std::cout << "norm pass" << std::endl;
	//	//Vertices
	//	glm::vec3& v0 = m_modelMesh->getVertices()[i + 0];
	//	glm::vec3& v1 = m_modelMesh->getVertices()[i + 1];
	//	glm::vec3& v2 = m_modelMesh->getVertices()[i + 2];
	//	
	//	glm::vec2& uv0 = m_modelMesh->getTextureCoords()[i + 0];
	//	glm::vec2& uv1 = m_modelMesh->getTextureCoords()[i + 1];
	//	glm::vec2& uv2 = m_modelMesh->getTextureCoords()[i + 2];
	//	
	//	//Edge of triangle
	//	glm::vec3 deltaPos1 = v1 - v0;
	//	glm::vec3 deltaPos2 = v2 - v0;
	//	
	//	//UV delta
	//	float deltaU1 = uv1.x - uv0.y;
	//	float deltaV1 = uv1.y - uv0.y;
	//	float deltaU2 = uv2.x - uv0.x;
	//	float deltaV2 = uv2.y - uv0.y;
	//	
	//	float f = 1.0f / (deltaU1 * deltaV2 - deltaU2 * deltaV1);
	//	
	//	glm::vec3 tangent;
	//	tangent.x = f * (deltaV2 * deltaPos1.x - deltaV1 * deltaPos2.x);
	//	tangent.y = f * (deltaV2 * deltaPos1.y - deltaV1 * deltaPos2.y);
	//	tangent.z = f * (deltaV2 * deltaPos1.z - deltaV1 * deltaPos2.z);
	//	
	//	glm::vec3 bitangent;
	//	bitangent.x = f * (-deltaU2 * deltaPos1.x + deltaU1 * deltaPos2.x);
	//	bitangent.y = f * (-deltaU2 * deltaPos1.y + deltaU1 * deltaPos2.y);
	//	bitangent.z = f * (-deltaU2 * deltaPos1.z + deltaU1 * deltaPos2.z);
	//	
	//	m_tangents.push_back(tangent);
	//	m_tangents.push_back(tangent);
	//	m_tangents.push_back(tangent);
	//	
	//	m_bitangents.push_back(bitangent);
	//	m_bitangents.push_back(bitangent);
	//	m_bitangents.push_back(bitangent);
	//
	//}
	//
	//glBindBuffer(GL_ARRAY_BUFFER, m_VBO[3]);
	//glBufferData(GL_ARRAY_BUFFER, m_tangents.size() * sizeof(glm::vec3), &m_tangents[0], GL_STATIC_DRAW);
	//
	//glBindBuffer(GL_ARRAY_BUFFER, m_VBO[4]);
	//glBufferData(GL_ARRAY_BUFFER, m_bitangents.size() * sizeof(glm::vec3), &m_bitangents[0], GL_STATIC_DRAW);

}
