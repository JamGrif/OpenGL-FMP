#include "ModelLighting.h"

ModelLighting::ModelLighting(glm::vec3 position, glm::vec3 rotation)
	:Model(position, rotation), m_modelDiffuseTexture(nullptr), m_modelSpecularTexture(nullptr),
	m_modelEmissionTexture(nullptr), m_modelNormalTexture(nullptr), m_modelHeightTexture(nullptr),
	m_normalizeTexture(false), m_usingEmission(false), m_usingNormal(false), m_usingHeight(false)
{

	m_localLightManager = EngineStatics::getLightManager();

	setShaderOne("res/shaders/lightingPassOne-vertex.glsl", "res/shaders/lightingPassOne-fragment.glsl");
	setShaderTwo("res/shaders/lightingPassTwo-vertex.glsl", "res/shaders/lightingPassTwo-fragment.glsl");
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

	if (m_modelHeightTexture != nullptr)
	{
		m_modelHeightTexture = nullptr;
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
	m_modelShaderPassTwo->setUniform1i("material.normal", 3);
	m_modelShaderPassTwo->setUniform1i("material.height", 4);
	m_modelShaderPassTwo->setUniform1f("material.shininess", 48.0f);

	m_modelShaderPassTwo->setUniform1i("material.normalizeTex", m_normalizeTexture);
	m_modelShaderPassTwo->setUniform1i("material.usingNormal", m_usingNormal);
	m_modelShaderPassTwo->setUniform1i("material.usingEmission", m_usingEmission);
	//m_modelShaderPassTwo->setUniform1i("material.usingDisplacement", m_usingDisplacement);

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

	if (m_modelHeightTexture != nullptr)
	{
		m_modelHeightTexture->Bind(4);
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
		setVBOAttrib(true, true, true, false, false); //No use for tangents / bitangents if no normal map
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

	if (m_modelHeightTexture != nullptr)
	{
		m_modelHeightTexture->Unbind();
	}

	if (m_modelShaderPassTwo != nullptr)
	{
		m_modelShaderPassTwo->Unbind();
	}

	

}

/// <summary>
/// Assigns specified texture to the model to be used for a diffuse map
/// </summary>
/// <param name="texturePath"></param>
void ModelLighting::setDiffuseTexture(const char* texturePath)
{
	m_modelDiffuseTexture = TextureManager::loadTexture(texturePath);
}

/// <summary>
/// Assigns specified texture to the model to be used for a specular map
/// </summary>
/// <param name="texturePath"></param>
void ModelLighting::setSpecularTexture(const char* texturePath)
{
	m_modelSpecularTexture = TextureManager::loadTexture(texturePath);
}

/// <summary>
/// Assigns specified texture to the model to be used for an emission map
/// </summary>
/// <param name="texturePath"></param>
void ModelLighting::setEmissionTexture(const char* texturePath)
{
	m_modelEmissionTexture = TextureManager::loadTexture(texturePath);
	m_usingEmission = true;
}

/// <summary>
/// Assigns specified texture to the model to be used for a normal map
/// </summary>
/// <param name="texturePath"></param>
/// <param name="normalize">Should the texture be normalized in the fragment shader</param>
void ModelLighting::setNormalTexture(const char* texturePath, bool normalize)
{
	m_modelNormalTexture = TextureManager::loadTexture(texturePath);
	m_normalizeTexture = normalize;
	m_usingNormal = true;
}

/// <summary>
/// Assigns specified texture to the model to be used for a height map
/// </summary>
/// <param name="texturePath"></param>
void ModelLighting::setHeightTexture(const char* texturePath)
{
	m_modelHeightTexture = TextureManager::loadTexture(texturePath);
	m_usingHeight = true;
}
