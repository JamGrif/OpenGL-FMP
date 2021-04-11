#include "Model.h"

Model::Model(glm::vec3 position, glm::vec3 rotation)
	:m_modelMesh(nullptr), m_modelShader(nullptr), m_modelDiffuseTexture(nullptr), m_modelSpecularTexture(nullptr),
	m_position{ position }, m_rotation{ rotation }, m_scale{ 1.0f,1.0f,1.0f },
	m_mMat{ 1.0f }, m_vMat{ 1.0f }, m_tMat{ 1.0f }, m_rMat{ 1.0f }, m_sMat{ 1.0f }
{
	//std::cout << "MESH->Initialized" << std::endl;

	m_localLightManager = EngineStatics::getLightManager();

}

Model::~Model()
{
	std::cout << "MESH->Destroyed" << std::endl;

	if (m_modelMesh != nullptr)
	{
		m_modelMesh = nullptr;
	}

	if (m_modelShader != nullptr)
	{
		m_modelShader = nullptr;
	}

	if (m_modelDiffuseTexture != nullptr)
	{
		m_modelDiffuseTexture = nullptr;
	}
	
	if (m_modelSpecularTexture != nullptr)
	{
		m_modelSpecularTexture = nullptr;
	}

}

void Model::drawModel()
{
	//If no valid model or shader attached
	if (m_modelMesh == nullptr || m_modelShader == nullptr)
	{
		return;
	}

	/*
		Set matrix values
	*/

	setMatrixValues();

	/*
		Set Shader Vertex values
	*/

	//Bind shader
	m_modelShader->Bind();

	m_modelShader->setUniformMatrix4fv("m_matrix", m_mMat);
	m_modelShader->setUniformMatrix4fv("v_matrix", m_vMat);
	m_modelShader->setUniformMatrix4fv("proj_matrix", *EngineStatics::getProjectionMatrix());

	/*
		Set Shader Fragment values
	*/

	if (m_modelShader->getVertexPath() == "res/shaders/lighting-vertex.glsl")
	{

		//Ensure a directional light exists
		if (m_localLightManager->getCurrentDirectionalLights() > 0)
		{
			m_modelShader->setUniform3f("dLight.ambient", m_localLightManager->getDirectionalLight(0)->Ambient);
			m_modelShader->setUniform3f("dLight.diffuse", m_localLightManager->getDirectionalLight(0)->Diffuse);
			m_modelShader->setUniform3f("dLight.specular", m_localLightManager->getDirectionalLight(0)->Specular);
			m_modelShader->setUniform3f("dLight.direction", m_localLightManager->getDirectionalLight(0)->Direction);
		}

		//Ensure a point light exists
		if (m_localLightManager->getCurrentPointLights() > 0)
		{
			//Point lights
			if (m_localLightManager->getCurrentPointLights() >= 1)
			{
				m_modelShader->setUniform3f("pLight[0].ambient", m_localLightManager->getPointLight(0)->Ambient);
				m_modelShader->setUniform3f("pLight[0].diffuse", m_localLightManager->getPointLight(0)->Diffuse);
				m_modelShader->setUniform3f("pLight[0].specular", m_localLightManager->getPointLight(0)->Specular);
				m_modelShader->setUniform3f("pLight[0].position", m_localLightManager->getPointLight(0)->Position);
				m_modelShader->setUniform1f("pLight[0].constant", m_localLightManager->getPointLight(0)->Constant);
				m_modelShader->setUniform1f("pLight[0].linear", m_localLightManager->getPointLight(0)->Linear);
				m_modelShader->setUniform1f("pLight[0].quadratic", m_localLightManager->getPointLight(0)->Quadratic);
			}

			if (m_localLightManager->getCurrentPointLights() >= 2)
			{
				m_modelShader->setUniform3f("pLight[1].ambient", m_localLightManager->getPointLight(1)->Ambient);
				m_modelShader->setUniform3f("pLight[1].diffuse", m_localLightManager->getPointLight(1)->Diffuse);
				m_modelShader->setUniform3f("pLight[1].specular", m_localLightManager->getPointLight(1)->Specular);
				m_modelShader->setUniform3f("pLight[1].position", m_localLightManager->getPointLight(1)->Position);
				m_modelShader->setUniform1f("pLight[1].constant", m_localLightManager->getPointLight(1)->Constant);
				m_modelShader->setUniform1f("pLight[1].linear", m_localLightManager->getPointLight(1)->Linear);
				m_modelShader->setUniform1f("pLight[1].quadratic", m_localLightManager->getPointLight(1)->Quadratic);
			}

			if (m_localLightManager->getCurrentPointLights() >= 3)
			{
				m_modelShader->setUniform3f("pLight[2].ambient", m_localLightManager->getPointLight(2)->Ambient);
				m_modelShader->setUniform3f("pLight[2].diffuse", m_localLightManager->getPointLight(2)->Diffuse);
				m_modelShader->setUniform3f("pLight[2].specular", m_localLightManager->getPointLight(2)->Specular);
				m_modelShader->setUniform3f("pLight[2].position", m_localLightManager->getPointLight(2)->Position);
				m_modelShader->setUniform1f("pLight[2].constant", m_localLightManager->getPointLight(2)->Constant);
				m_modelShader->setUniform1f("pLight[2].linear", m_localLightManager->getPointLight(2)->Linear);
				m_modelShader->setUniform1f("pLight[2].quadratic", m_localLightManager->getPointLight(2)->Quadratic);
			}

			if (m_localLightManager->getCurrentPointLights() >= 4)
			{
				m_modelShader->setUniform3f("pLight[3].ambient", m_localLightManager->getPointLight(3)->Ambient);
				m_modelShader->setUniform3f("pLight[3].diffuse", m_localLightManager->getPointLight(3)->Diffuse);
				m_modelShader->setUniform3f("pLight[3].specular", m_localLightManager->getPointLight(3)->Specular);
				m_modelShader->setUniform3f("pLight[3].position", m_localLightManager->getPointLight(3)->Position);
				m_modelShader->setUniform1f("pLight[3].constant", m_localLightManager->getPointLight(3)->Constant);
				m_modelShader->setUniform1f("pLight[3].linear", m_localLightManager->getPointLight(3)->Linear);
				m_modelShader->setUniform1f("pLight[3].quadratic", m_localLightManager->getPointLight(3)->Quadratic);
			}
		}

		//Ensure a spot light exists
		if (m_localLightManager->getCurrentSpotLights() > 0)
		{
			m_modelShader->setUniform3f("sLight.ambient", m_localLightManager->getSpotLight(0)->Ambient);
			m_modelShader->setUniform3f("sLight.diffuse", m_localLightManager->getSpotLight(0)->Diffuse);
			m_modelShader->setUniform3f("sLight.specular", m_localLightManager->getSpotLight(0)->Specular);
			m_modelShader->setUniform3f("sLight.position", EngineStatics::getCamera()->getPosition());
			m_modelShader->setUniform3f("sLight.direction", EngineStatics::getCamera()->getFront());
			m_modelShader->setUniform1f("sLight.cutOff", glm::cos(glm::radians(m_localLightManager->getSpotLight(0)->cutOff)));
			m_modelShader->setUniform1f("sLight.outerCutOff", glm::cos(glm::radians(m_localLightManager->getSpotLight(0)->outerCutOff)));
			m_modelShader->setUniform1f("sLight.constant", m_localLightManager->getSpotLight(0)->Constant);
			m_modelShader->setUniform1f("sLight.linear", m_localLightManager->getSpotLight(0)->Linear);
			m_modelShader->setUniform1f("sLight.quadratic", m_localLightManager->getSpotLight(0)->Quadratic);
		}


		//Material properties
		m_modelShader->setUniform1i("material.diffuse", 0);
		m_modelShader->setUniform1i("material.specular", 1);
		m_modelShader->setUniform1i("material.emission", 2);
		m_modelShader->setUniform1f("material.shininess", 32.0f);

		//Camera Position
		m_modelShader->setUniform3f("viewPos", EngineStatics::getCamera()->getPosition());



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

	}

	/*
		Bind VBOs and vertex attributes
	*/

	if (m_modelShader->getFragmentPath() == "res/shaders/lighting-fragment.glsl")
	{
		setVBOAttrib(true, true, true);
	}
	else 
	{
		setVBOAttrib(true, false, false);
	}


	//Draw
	glDrawArrays(GL_TRIANGLES, 0, m_modelMesh->getNumVertices());

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

	if (m_modelShader != nullptr)
	{
		m_modelShader->Unbind();
	}

}

void Model::setMesh(const char* meshFilePath)
{
	m_modelMesh = MeshManager::loadModel(meshFilePath);

	std::vector<glm::vec3> vert = m_modelMesh->getVertices();
	std::vector<glm::vec2> tex = m_modelMesh->getTextureCoords();
	std::vector<glm::vec3> norm = m_modelMesh->getNormals();

	std::vector<float> pvalues;     //Vertex positions
	std::vector<float> tvalues;     //Texture coordinates
	std::vector<float> nvalues;     //normal vectors

	for (int i = 0; i < m_modelMesh->getNumVertices(); i++)
	{
		pvalues.push_back((vert[i]).x);
		pvalues.push_back((vert[i]).y);
		pvalues.push_back((vert[i]).z);
		tvalues.push_back((tex[i]).s);
		tvalues.push_back((tex[i]).t);
		nvalues.push_back((norm[i]).x);
		nvalues.push_back((norm[i]).y);
		nvalues.push_back((norm[i]).z);
	}

	glGenBuffers(3, m_VBO);

	//Vertex locations
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);

	//Texture coordinates
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);

	//Normal vectors
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);
}

void Model::setShader(const char* vertexPath, const char* fragmentPath)
{
	m_modelShader = ShaderManager::loadShader(vertexPath, fragmentPath);
}

void Model::setDiffuseTexture(const char* texturePath)
{
	m_modelDiffuseTexture = TextureManager::loadTexture(texturePath);
}

void Model::setSpecularTexture(const char* texturePath)
{
	m_modelSpecularTexture = TextureManager::loadTexture(texturePath);
}

void Model::setEmissionTexture(const char* texturePath)
{
	m_modelEmissionTexture = TextureManager::loadTexture(texturePath);
}

void Model::SetXPos(float num) { m_position.x = num; }

void Model::SetYPos(float num) { m_position.y = num; }

void Model::SetZPos(float num) { m_position.z = num; }

float Model::GetXPos() { return m_position.x; }

float Model::GetYPos() { return m_position.y; }

float Model::GetZPos() { return m_position.z; }

void Model::SetXRot(float num) { m_position.x = num; }

void Model::SetYRot(float num) { m_position.y = num; }

void Model::SetZRot(float num) { m_position.z = num; }

float Model::GetXRot() { return m_rotation.x; }

float Model::GetYRot() { return m_rotation.y; }

float Model::GetZRot() { return m_rotation.z; }

void Model::SetXScale(float num) { m_scale.x = num; }

void Model::SetYScale(float num) { m_scale.y = num; }

void Model::SetZScale(float num) { m_scale.z = num; }

float Model::GetXScale() { return m_scale.x; }

float Model::GetYScale() { return m_scale.y; }

float Model::GetZScale() { return m_scale.z; }

void Model::IncXPos(float num) { m_position.x += num; }

void Model::IncYPos(float num) { m_position.y += num; }

void Model::IncZPos(float num) { m_position.z += num; }

void Model::DecXPos(float num) { m_position.x -= num; }

void Model::DecYPos(float num) { m_position.y -= num; }

void Model::DecZPos(float num) { m_position.z -= num; }

void Model::IncXRot(float num) { m_rotation.x += num; }

void Model::IncYRot(float num) { m_rotation.y += num; }

void Model::IncZRot(float num) { m_rotation.z += num; }

void Model::DecXRot(float num) { m_rotation.x -= num; }

void Model::DecYRot(float num) { m_rotation.y -= num; }

void Model::DecZRot(float num) { m_rotation.z -= num; }

void Model::IncXScale(float num) { m_scale.x += num; }

void Model::IncYScale(float num) { m_scale.y += num; }

void Model::IncZScale(float num) { m_scale.z += num; }

void Model::DecXScale(float num) { m_scale.x -= num; }

void Model::DecYScale(float num) { m_scale.y -= num; }

void Model::DecZScale(float num) { m_scale.z -= num; }

void Model::setMatrixValues()
{
	//Reset matrix values
	m_mMat = glm::mat4(1.0f);
	m_tMat = glm::mat4(1.0f);
	m_rMat = glm::mat4(1.0f);
	m_sMat = glm::mat4(1.0f);

	//Set meshes matrices
	m_tMat = glm::translate(m_tMat, m_position);
	m_rMat = glm::rotate(m_rMat, glm::radians(m_rotation.x), glm::vec3(1.0, 0.0f, 0.0f));
	m_rMat = glm::rotate(m_rMat, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0, 0.0f));
	m_rMat = glm::rotate(m_rMat, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	m_sMat = glm::scale(m_sMat, m_scale);

	m_mMat = m_tMat * m_rMat * m_sMat;
	m_vMat = EngineStatics::getCamera()->getViewMatrix();
}

void Model::setVBOAttrib(bool shaderPos, bool shaderTex, bool shaderNorm)
{
	if (shaderPos)
	{
		//Position
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
	}
	
	if (shaderTex)
	{
		//Texture
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO[1]);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);
	}
	
	if (shaderNorm)
	{
		//Normal
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO[2]);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);
	}
}



