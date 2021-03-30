#include "Mesh.h"

Mesh::Mesh(const char* meshFilePath)
	:m_meshModel(nullptr), m_meshShader(nullptr), m_meshDiffuseTexture(nullptr), m_meshSpecularTexture(nullptr),
	m_position{ 0.0f,0.0f,0.0f }, m_rotation{ 0.0f,0.0f,0.0f }, m_scale{ 1.0f,1.0f,1.0f },
	m_mMat{ 1.0f }, m_vMat{ 1.0f }, m_tMat{ 1.0f }, m_rMat{ 1.0f }, m_sMat{ 1.0f }, m_invTrMat{ 1.0f }
{
	//std::cout << "MESH->Initialized" << std::endl;

	m_localLightManager = EngineStatics::getLightManager();

	initMesh(meshFilePath);

}

Mesh::~Mesh()
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

void Mesh::initMesh(const char* meshFilePath)
{

	m_meshModel = MeshManager::loadModel(meshFilePath);
	m_meshShader = ShaderManager::loadShader("res/shaders/DEFAULT-vertexShader.glsl", "res/shaders/DEFAULT-fragmentShader.glsl");

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

void Mesh::updateMesh()
{
	drawMesh();
}

void Mesh::drawMesh()
{
	//Bind shader
	m_meshShader->Use();

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


	//Set shader uniform locations
	//Vertex
	glUniformMatrix4fv(glGetUniformLocation(m_meshShader->getProgram(), "m_matrix"), 1, GL_FALSE, glm::value_ptr(m_mMat));
	glUniformMatrix4fv(glGetUniformLocation(m_meshShader->getProgram(), "v_matrix"), 1, GL_FALSE, glm::value_ptr(m_vMat));
	glUniformMatrix4fv(glGetUniformLocation(m_meshShader->getProgram(), "proj_matrix"), 1, GL_FALSE, glm::value_ptr(*EngineStatics::getProjectionMatrix()));


	//Light properties
	//glUniform3f(glGetUniformLocation(m_meshShader->getProgram(), "dLight.ambient"), m_localLightManager->getDirectionalLight()->Ambient.r, m_localLightManager->getDirectionalLight()->Ambient.g, m_localLightManager->getDirectionalLight()->Ambient.b);
	//glUniform3f(glGetUniformLocation(m_meshShader->getProgram(), "dLight.diffuse"), m_localLightManager->getDirectionalLight()->Diffuse.r, m_localLightManager->getDirectionalLight()->Diffuse.g, m_localLightManager->getDirectionalLight()->Diffuse.b);
	//glUniform3f(glGetUniformLocation(m_meshShader->getProgram(), "dLight.specular"), m_localLightManager->getDirectionalLight()->Specular.r, m_localLightManager->getDirectionalLight()->Specular.g, m_localLightManager->getDirectionalLight()->Specular.b);
	//glUniform3f(glGetUniformLocation(m_meshShader->getProgram(), "dLight.direction"), m_localLightManager->getDirectionalLight()->Direction.x, m_localLightManager->getDirectionalLight()->Direction.y, m_localLightManager->getDirectionalLight()->Direction.z);

	//Point lights
	glUniform3f(glGetUniformLocation(m_meshShader->getProgram(), "pLight.ambient"), m_localLightManager->getPointLight(0)->Ambient.r, m_localLightManager->getPointLight(0)->Ambient.g, m_localLightManager->getPointLight(0)->Ambient.b);
	glUniform3f(glGetUniformLocation(m_meshShader->getProgram(), "pLight.diffuse"), m_localLightManager->getPointLight(0)->Diffuse.r, m_localLightManager->getPointLight(0)->Diffuse.g, m_localLightManager->getPointLight(0)->Diffuse.b);
	glUniform3f(glGetUniformLocation(m_meshShader->getProgram(), "pLight.specular"), m_localLightManager->getPointLight(0)->Specular.r, m_localLightManager->getPointLight(0)->Specular.g, m_localLightManager->getPointLight(0)->Specular.b);
	glUniform3f(glGetUniformLocation(m_meshShader->getProgram(), "pLight.position"), EngineStatics::getCamera()->getPosition().x, EngineStatics::getCamera()->getPosition().y, EngineStatics::getCamera()->getPosition().z);
	glUniform1f(glGetUniformLocation(m_meshShader->getProgram(), "pLight.constant"), m_localLightManager->getPointLight(0)->Constant);
	glUniform1f(glGetUniformLocation(m_meshShader->getProgram(), "pLight.linear"), m_localLightManager->getPointLight(0)->Linear);
	glUniform1f(glGetUniformLocation(m_meshShader->getProgram(), "pLight.quadratic"), m_localLightManager->getPointLight(0)->Quadratic);



	//Material properties
	glUniform1i(glGetUniformLocation(m_meshShader->getProgram(), "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(m_meshShader->getProgram(), "material.specular"), 1);
	glUniform1f(glGetUniformLocation(m_meshShader->getProgram(), "material.shininess"), 48.0f);


	glUniform3f(glGetUniformLocation(m_meshShader->getProgram(), "viewPos"), EngineStatics::getCamera()->getPosition().x, EngineStatics::getCamera()->getPosition().y, EngineStatics::getCamera()->getPosition().z);

	//Bind textures to pipeline
	if (m_meshDiffuseTexture != nullptr)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_meshDiffuseTexture->getTexture());
	}
	//No diffuse texture
	else
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	if (m_meshSpecularTexture != nullptr)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_meshSpecularTexture->getTexture());
	}
	//No specular texture
	else
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	//Activate VBO and associate vertex attributes
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[2]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glDrawArrays(GL_TRIANGLES, 0, m_meshModel->getNumVertices());

}

void Mesh::loadDiffuseTexture(const char* texturePath)
{
	m_meshDiffuseTexture = TextureManager::loadTexture(texturePath);
}

void Mesh::loadSpecularTexture(const char* texturePath)
{
	m_meshSpecularTexture = TextureManager::loadTexture(texturePath);
}

void Mesh::SetXPos(float num) { m_position.x = num; }

void Mesh::SetYPos(float num) { m_position.y = num; }

void Mesh::SetZPos(float num) { m_position.z = num; }

float Mesh::GetXPos() { return m_position.x; }

float Mesh::GetYPos() { return m_position.y; }

float Mesh::GetZPos() { return m_position.z; }

void Mesh::SetXRot(float num) { m_position.x = num; }

void Mesh::SetYRot(float num) { m_position.y = num; }

void Mesh::SetZRot(float num) { m_position.z = num; }

float Mesh::GetXRot() { return m_rotation.x; }

float Mesh::GetYRot() { return m_rotation.y; }

float Mesh::GetZRot() { return m_rotation.z; }

void Mesh::SetXScale(float num) { m_scale.x = num; }

void Mesh::SetYScale(float num) { m_scale.y = num; }

void Mesh::SetZScale(float num) { m_scale.z = num; }

float Mesh::GetXScale() { return m_scale.x; }

float Mesh::GetYScale() { return m_scale.y; }

float Mesh::GetZScale() { return m_scale.z; }

void Mesh::IncXPos(float num) { m_position.x += num; }

void Mesh::IncYPos(float num) { m_position.y += num; }

void Mesh::IncZPos(float num) { m_position.z += num; }

void Mesh::DecXPos(float num) { m_position.x -= num; }

void Mesh::DecYPos(float num) { m_position.y -= num; }

void Mesh::DecZPos(float num) { m_position.z -= num; }

void Mesh::IncXRot(float num) { m_rotation.x += num; }

void Mesh::IncYRot(float num) { m_rotation.y += num; }

void Mesh::IncZRot(float num) { m_rotation.z += num; }

void Mesh::DecXRot(float num) { m_rotation.x -= num; }

void Mesh::DecYRot(float num) { m_rotation.y -= num; }

void Mesh::DecZRot(float num) { m_rotation.z -= num; }

void Mesh::IncXScale(float num) { m_scale.x += num; }

void Mesh::IncYScale(float num) { m_scale.y += num; }

void Mesh::IncZScale(float num) { m_scale.z += num; }

void Mesh::DecXScale(float num) { m_scale.x -= num; }

void Mesh::DecYScale(float num) { m_scale.y -= num; }

void Mesh::DecZScale(float num) { m_scale.z -= num; }
