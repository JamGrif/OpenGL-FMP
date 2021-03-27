#include "Mesh.h"

Mesh::Mesh(const char* meshFilePath)
	:meshModel(nullptr), meshShader(nullptr), meshDiffuseTexture(0), meshSpecularTexture(0),
	m_position{ 0.0f,0.0f,0.0f }, m_rotation{ 0.0f,0.0f,0.0f }, m_scale{ 1.0f,1.0f,1.0f },
	mMat{ 1.0f }, vMat{ 1.0f }, tMat{ 1.0f }, rMat{ 1.0f }, sMat{ 1.0f }, invTrMat{ 1.0f }
{
	std::cout << "Mesh Initialized" << std::endl;

	initMesh(meshFilePath);
	//std::cout << meshModel << std::endl;
}

Mesh::~Mesh()
{
	std::cout << "Mesh Destroyed" << std::endl;

	//delete meshModel;
	meshModel = nullptr;

	delete meshShader;
	meshShader = nullptr;


}

void Mesh::initMesh(const char* meshFilePath)
{
	//modelMaterial.Ambient = Utility::goldAmbient();
	//modelMaterial.Diffuse = Utility::goldDiffuse();
	//modelMaterial.Specular = Utility::goldSpecular();
	//modelMaterial.Shininess = Utility::goldShininess();

	meshModel = MeshManager::loadModel(meshFilePath);
	//meshModel = new ImportedModel(meshFilePath);
	meshShader = new Shader("res/shaders/DEFAULT-vertexShader.glsl", "res/shaders/DEFAULT-fragmentShader.glsl");
	//std::cout << "hello" << std::endl;
	std::vector<glm::vec3> vert = meshModel->getVertices();
	std::vector<glm::vec2> tex = meshModel->getTextureCoords();
	std::vector<glm::vec3> norm = meshModel->getNormals();

	std::vector<float> pvalues;     //Vertex positions
	std::vector<float> tvalues;     //Texture coordinates
	std::vector<float> nvalues;     //normal vectors

	for (int i = 0; i < meshModel->getNumVertices(); i++)
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

	glGenBuffers(3, VBO);


	//Vertex locations
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);

	//Texture coordinates
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);

	//Normal vectors
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);

	//meshDiffuseTexture = loadDiffuseTexture("res/textures/shuttle_diff.jpg");
	//meshSpecularTexture = loadDiffuseTexture("res/textures/shuttle_spec.jpg");

}

void Mesh::updateMesh()
{
	//std::cout << "updatemesh" << std::endl;
	drawMesh();
}

void Mesh::drawMesh()
{
	//Bind shader
	meshShader->Use();

	//Reset matrix values
	mMat = glm::mat4(1.0f);
	tMat = glm::mat4(1.0f);
	rMat = glm::mat4(1.0f);
	sMat = glm::mat4(1.0f);

	//Find uniform locations
	//Vertex
	mLoc = glGetUniformLocation(meshShader->getProgram(), "m_matrix");
	vLoc = glGetUniformLocation(meshShader->getProgram(), "v_matrix");
	projLoc = glGetUniformLocation(meshShader->getProgram(), "proj_matrix");


	//Light properties
	glUniform3f(glGetUniformLocation(meshShader->getProgram(), "pLight.ambient"), 0.2f, 0.2f, 0.2f);
	glUniform3f(glGetUniformLocation(meshShader->getProgram(), "pLight.diffuse"), 0.5f, 0.5f, 0.5f);
	glUniform3f(glGetUniformLocation(meshShader->getProgram(), "pLight.specular"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(meshShader->getProgram(), "pLight.position"), EngineStatics::getLightManager()->getPointLight()->Position.x, EngineStatics::getLightManager()->getPointLight()->Position.y, EngineStatics::getLightManager()->getPointLight()->Position.z);

	//Material properties
	glUniform1i(glGetUniformLocation(meshShader->getProgram(), "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(meshShader->getProgram(), "material.specular"), 1);
	glUniform1f(glGetUniformLocation(meshShader->getProgram(), "material.shininess"), 48.0f);

	//Bind diffuse map
	if (meshDiffuseTexture > 0)
	{
		//std::cout << meshDiffuseTexture << std::endl;
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, meshDiffuseTexture);

	//Bind specular map
	if (meshSpecularTexture > 0)
	{
		//std::cout << meshSpecularTexture << std::endl;
	}

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, meshSpecularTexture);

	//Fragment
	viewPosLoc = glGetUniformLocation(meshShader->getProgram(), "viewPos");

	
	//Set matrices
	tMat = glm::translate(tMat, m_position);
	rMat = glm::rotate(rMat, glm::radians(m_rotation.x), glm::vec3(1.0, 0.0f, 0.0f));
	rMat = glm::rotate(rMat, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0, 0.0f));
	rMat = glm::rotate(rMat, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	sMat = glm::scale(sMat, m_scale);

	mMat = tMat * rMat * sMat;
	vMat = EngineStatics::getCamera()->getViewMatrix();

	//Set uniform values
	glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(mMat));
	glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(vMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(*EngineStatics::getProjectionMatrix()));
	

	glUniform3f(viewPosLoc, EngineStatics::getCamera()->getPosition().x, EngineStatics::getCamera()->getPosition().y, EngineStatics::getCamera()->getPosition().z);


	//Activate VBO and associate vertex attributes
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	//glActiveTexture(GL_TEXTURE0); //Make 0th texture unit active in shader
	//glBindTexture(GL_TEXTURE_2D, meshDiffuseTexture);


	glDrawArrays(GL_TRIANGLES, 0, meshModel->getNumVertices());
}

void Mesh::loadDiffuseTexture(const char* texturePath)
{
	GLuint textureID;
	textureID = SOIL_load_OGL_texture(texturePath, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y); //Generates texture object

	if (textureID == 0)
	{
		std::cout << "Could not find requested texture! " << texturePath << std::endl;
		return;
	}

	glBindTexture(GL_TEXTURE_2D, textureID); //Activate texture

	//Mipmap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); //Apply Mipmapping
	glGenerateMipmap(GL_TEXTURE_2D);

	//Anisotropic filtering
	if (glewIsSupported("GL_EXT_texture_filter_anisotropic")) //Ensure supported
	{
		GLfloat anisoSetting = 0.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisoSetting); //Set anisoSetting to maximum sampling support
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisoSetting); //Apply Anisotropic Filtering
	}

	//Specify what happens to texture coords outside 0-1 range
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	meshDiffuseTexture = textureID;
}

void Mesh::loadSpecularTexture(const char* texturePath)
{
	GLuint textureID;
	textureID = SOIL_load_OGL_texture(texturePath, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y); //Generates texture object

	if (textureID == 0)
	{
		std::cout << "Could not find requested texture! " << texturePath << std::endl;
		return;
	}

	glBindTexture(GL_TEXTURE_2D, textureID); //Activate texture

	//Mipmap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); //Apply Mipmapping
	glGenerateMipmap(GL_TEXTURE_2D);

	//Anisotropic filtering
	if (glewIsSupported("GL_EXT_texture_filter_anisotropic")) //Ensure supported
	{
		GLfloat anisoSetting = 0.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisoSetting); //Set anisoSetting to maximum sampling support
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisoSetting); //Apply Anisotropic Filtering
	}

	//Specify what happens to texture coords outside 0-1 range
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	meshSpecularTexture = textureID;
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
