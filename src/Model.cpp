#include "Model.h"

Model::Model()
	:m_meshModel(nullptr), m_meshShader(nullptr), m_meshDiffuseTexture(nullptr), m_meshSpecularTexture(nullptr),
	m_position{ 0.0f,0.0f,0.0f }, m_rotation{ 0.0f,0.0f,0.0f }, m_scale{ 1.0f,1.0f,1.0f },
	m_mMat{ 1.0f }, m_vMat{ 1.0f }, m_tMat{ 1.0f }, m_rMat{ 1.0f }, m_sMat{ 1.0f }, m_invTrMat{ 1.0f }
{
	//std::cout << "MESH->Initialized" << std::endl;

	m_localLightManager = EngineStatics::getLightManager();

	m_meshShader = ShaderManager::loadShader("res/shaders/DEFAULT-vertexShader.glsl", "res/shaders/DEFAULT-fragmentShader.glsl");
	//initMesh(meshFilePath);

}

Model::~Model()
{
	std::cout << "MESH->Destroyed" << std::endl;

	if (m_meshModel != nullptr)
	{
		m_meshModel = nullptr;
	}

	if (m_meshShader != nullptr)
	{
		m_meshShader = nullptr;
	}

	if (m_meshDiffuseTexture != nullptr)
	{
		m_meshDiffuseTexture = nullptr;
	}
	
	if (m_meshSpecularTexture != nullptr)
	{
		m_meshSpecularTexture = nullptr;
	}

}

void Model::setMesh(const char* meshFilePath)
{
	m_meshModel = MeshManager::loadModel(meshFilePath);


	std::vector<glm::vec3> vert = m_meshModel->getVertices();
	std::vector<glm::vec2> tex = m_meshModel->getTextureCoords();
	std::vector<glm::vec3> norm = m_meshModel->getNormals();

	std::vector<float> pvalues;     //Vertex positions
	std::vector<float> tvalues;     //Texture coordinates
	std::vector<float> nvalues;     //normal vectors

	for (int i = 0; i < m_meshModel->getNumVertices(); i++)
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




void Model::drawModel()
{
	//If no valid model or shader attached
	if (m_meshModel == nullptr || m_meshShader == nullptr)
	{
		return;
	}
	
	/*
		Set matrix values
	*/

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

	/*
		Set Shader Vertex values
	*/

	//Bind shader
	m_meshShader->Use();

	
	glUniformMatrix4fv(glGetUniformLocation(m_meshShader->getProgram(), "m_matrix"), 1, GL_FALSE, glm::value_ptr(m_mMat));
	glUniformMatrix4fv(glGetUniformLocation(m_meshShader->getProgram(), "v_matrix"), 1, GL_FALSE, glm::value_ptr(m_vMat));
	glUniformMatrix4fv(glGetUniformLocation(m_meshShader->getProgram(), "proj_matrix"), 1, GL_FALSE, glm::value_ptr(*EngineStatics::getProjectionMatrix()));


	/*
		Set Shader Fragment values
	*/

	//Ensure a directional light exists
	if (m_localLightManager->getCurrentDirectionalLights() > 0)
	{
		glUniform3f(glGetUniformLocation(m_meshShader->getProgram(), "dLight.ambient"), m_localLightManager->getDirectionalLight(0)->Ambient.r, m_localLightManager->getDirectionalLight(0)->Ambient.g, m_localLightManager->getDirectionalLight(0)->Ambient.b);
		glUniform3f(glGetUniformLocation(m_meshShader->getProgram(), "dLight.diffuse"), m_localLightManager->getDirectionalLight(0)->Diffuse.r, m_localLightManager->getDirectionalLight(0)->Diffuse.g, m_localLightManager->getDirectionalLight(0)->Diffuse.b);
		glUniform3f(glGetUniformLocation(m_meshShader->getProgram(), "dLight.specular"), m_localLightManager->getDirectionalLight(0)->Specular.r, m_localLightManager->getDirectionalLight(0)->Specular.g, m_localLightManager->getDirectionalLight(0)->Specular.b);
		glUniform3f(glGetUniformLocation(m_meshShader->getProgram(), "dLight.direction"), m_localLightManager->getDirectionalLight(0)->Direction.x, m_localLightManager->getDirectionalLight(0)->Direction.y, m_localLightManager->getDirectionalLight(0)->Direction.z);
	}

	//Ensure a point light exists
	if (m_localLightManager->getCurrentPointLights() > 0)
	{
		//Point lights
		if (m_localLightManager->getCurrentPointLights() >= 1)
		{
			glUniform3f(glGetUniformLocation(m_meshShader->getProgram(), ("pLight[0].ambient")), m_localLightManager->getPointLight(0)->Ambient.r, m_localLightManager->getPointLight(0)->Ambient.g, m_localLightManager->getPointLight(0)->Ambient.b);
			glUniform3f(glGetUniformLocation(m_meshShader->getProgram(), ("pLight[0].diffuse")), m_localLightManager->getPointLight(0)->Diffuse.r, m_localLightManager->getPointLight(0)->Diffuse.g, m_localLightManager->getPointLight(0)->Diffuse.b);
			glUniform3f(glGetUniformLocation(m_meshShader->getProgram(), ("pLight[0].specular")), m_localLightManager->getPointLight(0)->Specular.r, m_localLightManager->getPointLight(0)->Specular.g, m_localLightManager->getPointLight(0)->Specular.b);
			glUniform3f(glGetUniformLocation(m_meshShader->getProgram(), ("pLight[0].position")), m_localLightManager->getPointLight(0)->Position.x, m_localLightManager->getPointLight(0)->Position.y, m_localLightManager->getPointLight(0)->Position.z);
			glUniform1f(glGetUniformLocation(m_meshShader->getProgram(), ("pLight[0].constant")), m_localLightManager->getPointLight(0)->Constant);
			glUniform1f(glGetUniformLocation(m_meshShader->getProgram(), ("pLight[0].linear")), m_localLightManager->getPointLight(0)->Linear);
			glUniform1f(glGetUniformLocation(m_meshShader->getProgram(), ("pLight[0].quadratic")), m_localLightManager->getPointLight(0)->Quadratic);
		}

		if (m_localLightManager->getCurrentPointLights() >= 2)
		{
			glUniform3f(glGetUniformLocation(m_meshShader->getProgram(), ("pLight[1].ambient")), m_localLightManager->getPointLight(1)->Ambient.r, m_localLightManager->getPointLight(1)->Ambient.g, m_localLightManager->getPointLight(1)->Ambient.b);
			glUniform3f(glGetUniformLocation(m_meshShader->getProgram(), ("pLight[1].diffuse")), m_localLightManager->getPointLight(1)->Diffuse.r, m_localLightManager->getPointLight(1)->Diffuse.g, m_localLightManager->getPointLight(1)->Diffuse.b);
			glUniform3f(glGetUniformLocation(m_meshShader->getProgram(), ("pLight[1].specular")), m_localLightManager->getPointLight(1)->Specular.r, m_localLightManager->getPointLight(1)->Specular.g, m_localLightManager->getPointLight(1)->Specular.b);
			glUniform3f(glGetUniformLocation(m_meshShader->getProgram(), ("pLight[1].position")), m_localLightManager->getPointLight(1)->Position.x, m_localLightManager->getPointLight(1)->Position.y, m_localLightManager->getPointLight(1)->Position.z);
			glUniform1f(glGetUniformLocation(m_meshShader->getProgram(), ("pLight[1].constant")), m_localLightManager->getPointLight(1)->Constant);
			glUniform1f(glGetUniformLocation(m_meshShader->getProgram(), ("pLight[1].linear")), m_localLightManager->getPointLight(1)->Linear);
			glUniform1f(glGetUniformLocation(m_meshShader->getProgram(), ("pLight[1].quadratic")), m_localLightManager->getPointLight(1)->Quadratic);
		}

		if (m_localLightManager->getCurrentPointLights() >= 3)
		{
			glUniform3f(glGetUniformLocation(m_meshShader->getProgram(), ("pLight[2].ambient")), m_localLightManager->getPointLight(2)->Ambient.r, m_localLightManager->getPointLight(2)->Ambient.g, m_localLightManager->getPointLight(2)->Ambient.b);
			glUniform3f(glGetUniformLocation(m_meshShader->getProgram(), ("pLight[2].diffuse")), m_localLightManager->getPointLight(2)->Diffuse.r, m_localLightManager->getPointLight(2)->Diffuse.g, m_localLightManager->getPointLight(2)->Diffuse.b);
			glUniform3f(glGetUniformLocation(m_meshShader->getProgram(), ("pLight[2].specular")), m_localLightManager->getPointLight(2)->Specular.r, m_localLightManager->getPointLight(2)->Specular.g, m_localLightManager->getPointLight(2)->Specular.b);
			glUniform3f(glGetUniformLocation(m_meshShader->getProgram(), ("pLight[2].position")), m_localLightManager->getPointLight(2)->Position.x, m_localLightManager->getPointLight(2)->Position.y, m_localLightManager->getPointLight(2)->Position.z);
			glUniform1f(glGetUniformLocation(m_meshShader->getProgram(), ("pLight[2].constant")), m_localLightManager->getPointLight(2)->Constant);
			glUniform1f(glGetUniformLocation(m_meshShader->getProgram(), ("pLight[2].linear")), m_localLightManager->getPointLight(2)->Linear);
			glUniform1f(glGetUniformLocation(m_meshShader->getProgram(), ("pLight[2].quadratic")), m_localLightManager->getPointLight(2)->Quadratic);
		}

		if (m_localLightManager->getCurrentPointLights() >= 4)
		{
			glUniform3f(glGetUniformLocation(m_meshShader->getProgram(), ("pLight[3].ambient")), m_localLightManager->getPointLight(3)->Ambient.r, m_localLightManager->getPointLight(3)->Ambient.g, m_localLightManager->getPointLight(3)->Ambient.b);
			glUniform3f(glGetUniformLocation(m_meshShader->getProgram(), ("pLight[3].diffuse")), m_localLightManager->getPointLight(3)->Diffuse.r, m_localLightManager->getPointLight(3)->Diffuse.g, m_localLightManager->getPointLight(3)->Diffuse.b);
			glUniform3f(glGetUniformLocation(m_meshShader->getProgram(), ("pLight[3].specular")), m_localLightManager->getPointLight(3)->Specular.r, m_localLightManager->getPointLight(3)->Specular.g, m_localLightManager->getPointLight(3)->Specular.b);
			glUniform3f(glGetUniformLocation(m_meshShader->getProgram(), ("pLight[3].position")), m_localLightManager->getPointLight(3)->Position.x, m_localLightManager->getPointLight(3)->Position.y, m_localLightManager->getPointLight(3)->Position.z);
			glUniform1f(glGetUniformLocation(m_meshShader->getProgram(), ("pLight[3].constant")), m_localLightManager->getPointLight(3)->Constant);
			glUniform1f(glGetUniformLocation(m_meshShader->getProgram(), ("pLight[3].linear")), m_localLightManager->getPointLight(3)->Linear);
			glUniform1f(glGetUniformLocation(m_meshShader->getProgram(), ("pLight[3].quadratic")), m_localLightManager->getPointLight(3)->Quadratic);
		}
	}
			
	//Ensure a spot light exists
	if (m_localLightManager->getCurrentSpotLights() > 0)
	{
		glUniform3f(glGetUniformLocation(m_meshShader->getProgram(), "sLight.ambient"), m_localLightManager->getSpotLight(0)->Ambient.r, m_localLightManager->getSpotLight(0)->Ambient.g, m_localLightManager->getSpotLight(0)->Ambient.b);
		glUniform3f(glGetUniformLocation(m_meshShader->getProgram(), "sLight.diffuse"), m_localLightManager->getSpotLight(0)->Diffuse.r, m_localLightManager->getSpotLight(0)->Diffuse.g, m_localLightManager->getSpotLight(0)->Diffuse.b);
		glUniform3f(glGetUniformLocation(m_meshShader->getProgram(), "sLight.specular"), m_localLightManager->getSpotLight(0)->Specular.r, m_localLightManager->getSpotLight(0)->Specular.g, m_localLightManager->getSpotLight(0)->Specular.b);
		glUniform3f(glGetUniformLocation(m_meshShader->getProgram(), "sLight.position"), EngineStatics::getCamera()->getPosition().x, EngineStatics::getCamera()->getPosition().y, EngineStatics::getCamera()->getPosition().z);
		glUniform3f(glGetUniformLocation(m_meshShader->getProgram(), "sLight.direction"), EngineStatics::getCamera()->getFront().x, EngineStatics::getCamera()->getFront().y, EngineStatics::getCamera()->getFront().z);
		glUniform1f(glGetUniformLocation(m_meshShader->getProgram(), "sLight.cutOff"), glm::cos(glm::radians(m_localLightManager->getSpotLight(0)->cutOff)));
		glUniform1f(glGetUniformLocation(m_meshShader->getProgram(), "sLight.outerCutOff"), glm::cos(glm::radians(m_localLightManager->getSpotLight(0)->outerCutOff)));
		glUniform1f(glGetUniformLocation(m_meshShader->getProgram(), "sLight.constant"), m_localLightManager->getSpotLight(0)->Constant);
		glUniform1f(glGetUniformLocation(m_meshShader->getProgram(), "sLight.linear"), m_localLightManager->getSpotLight(0)->Linear);
		glUniform1f(glGetUniformLocation(m_meshShader->getProgram(), "sLight.quadratic"), m_localLightManager->getSpotLight(0)->Quadratic);
	}

	
	//Material properties
	glUniform1i(glGetUniformLocation(m_meshShader->getProgram(), "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(m_meshShader->getProgram(), "material.specular"), 1);
	glUniform1f(glGetUniformLocation(m_meshShader->getProgram(), "material.shininess"), 48.0f);

	//Camera Position
	glUniform3f(glGetUniformLocation(m_meshShader->getProgram(), "viewPos"), EngineStatics::getCamera()->getPosition().x, EngineStatics::getCamera()->getPosition().y, EngineStatics::getCamera()->getPosition().z);

	//Bind textures to pipeline
	if (m_meshDiffuseTexture != nullptr)
	{
		m_meshDiffuseTexture->Bind(0);
	}

	if (m_meshSpecularTexture != nullptr)
	{
		m_meshSpecularTexture->Bind(1);
	}

	/*
		Bind VBOs and vertex attributes
	*/

	//Position
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	//Texture
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	//Normal
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[2]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glDrawArrays(GL_TRIANGLES, 0, m_meshModel->getNumVertices());

	if (m_meshDiffuseTexture != nullptr)
	{
		m_meshDiffuseTexture->Unbind();
	}

	if (m_meshSpecularTexture != nullptr)
	{
		m_meshSpecularTexture->Unbind();
	}
	
	

}

void Model::setDiffuseTexture(const char* texturePath)
{
	m_meshDiffuseTexture = TextureManager::loadTexture(texturePath);
}

void Model::setSpecularTexture(const char* texturePath)
{
	m_meshSpecularTexture = TextureManager::loadTexture(texturePath);
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
