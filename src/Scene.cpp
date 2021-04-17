#include "Scene.h"

Scene::Scene()
	:m_sceneCamera(nullptr), m_sceneLightManager(nullptr)
{
	std::cout << "Scene Initialized" << std::endl;
}

Scene::~Scene()
{
	std::cout << "Scene Destroyed" << std::endl;
	
	for (Model* m : m_sceneMeshes)
	{
		delete m;
	}
	m_sceneMeshes.clear();


	delete m_sceneCamera;
	m_sceneCamera = nullptr;

	delete m_sceneLightManager;
	m_sceneLightManager = nullptr;


}

void Scene::initScene()
{
	setupShadowObjects();
	
	addSceneCamera(0.0f, 2.0f, 0.0f);
	addSceneLightManager();
	
	//m_sceneLightManager->addDirectionalLight(0.0f, -5.0f, -10.0f);
	
	//m_sceneLightManager->addSpotLight(0.0f, 0.0f, 0.0f);

	//Floor
	std::vector<glm::vec3> FloorPosRot =
	{
		glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(6.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-6.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 6.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(6.0f, 0.0f, 6.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-6.0f, 0.0f, 6.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -6.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(6.0f, 0.0f, -6.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-6.0f, 0.0f, -6.0f), glm::vec3(0.0f, 0.0f, 0.0f)
	};

	
	for (int i = 0; i < FloorPosRot.size(); i += 2)
	{
		Model* Floor = new Model(FloorPosRot.at(i), FloorPosRot.at(i + 1));
		Floor->setMesh("res/meshes/plane.obj");
		Floor->setShaderOne("res/shaders/lightingPassOne-vertex.glsl", "res/shaders/lightingPassOne-fragment.glsl");
		Floor->setShaderTwo("res/shaders/lightingPassTwo-vertex.glsl", "res/shaders/lightingPassTwo-fragment.glsl");
		Floor->setDiffuseTexture("res/textures/concrete_diff.png");
		Floor->setSpecularTexture("res/textures/concrete_spec.png");
		//Floor->setEmissionTexture("res/textures/matrix_emis.png");
		m_sceneMeshes.push_back(Floor);
	}
	
	//Z wall
	std::vector<glm::vec3> WallPosRot =
	{
		glm::vec3(-6.0f, 3.0f, -9.0f), glm::vec3(90.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 3.0f, -9.0f), glm::vec3(90.0f, 0.0f, 0.0f),
		glm::vec3(6.0f, 3.0f, -9.0f), glm::vec3(90.0f, 0.0f, 0.0f),
		glm::vec3(-6.0f, 3.0f, 9.0f), glm::vec3(180.0f, 90.0f, 90.0f),
		glm::vec3(0.0f, 3.0f, 9.0f), glm::vec3(180.0f, 90.0f, 90.0f),
		glm::vec3(6.0f, 3.0f, 9.0f), glm::vec3(180.0f, 90.0f, 90.0f)
	
	};
	
	for (int i = 0; i < WallPosRot.size(); i += 2)
	{
		Model* wall = new Model(WallPosRot.at(i), WallPosRot.at(i+1));
		wall->setMesh("res/meshes/plane.obj");
		wall->setShaderOne("res/shaders/lightingPassOne-vertex.glsl", "res/shaders/lightingPassOne-fragment.glsl");
		wall->setShaderTwo("res/shaders/lightingPassTwo-vertex.glsl", "res/shaders/lightingPassTwo-fragment.glsl");
		wall->setDiffuseTexture("res/textures/wood_diff.png");
		wall->setSpecularTexture("res/textures/wood_spec.png");
		m_sceneMeshes.push_back(wall);
	}

	//Side wall
	std::vector<glm::vec3> SideWallPosRot =
	{
		glm::vec3(-9.0f, 3.0f, -6.0f), glm::vec3(90.0f, 0.0f, -90.0f),
		glm::vec3(-9.0f, 3.0f, 0.0f), glm::vec3(90.0f, 0.0f, -90.0f),
		glm::vec3(-9.0f, 3.0f, 6.0f), glm::vec3(90.0f, 0.0f, -90.0f),
		glm::vec3(9.0f, 3.0f, -6.0f), glm::vec3(90.0f, 0.0f, 90.0f),
		glm::vec3(9.0f, 3.0f, 0.0f), glm::vec3(90.0f, 0.0f, 90.0f),
		glm::vec3(9.0f, 3.0f, 6.0f), glm::vec3(90.0f, 0.0f, 90.0f)
	};
	
	for (int i = 0; i < SideWallPosRot.size(); i += 2)
	{
		Model* wall = new Model(SideWallPosRot.at(i), SideWallPosRot.at(i + 1));
		wall->setMesh("res/meshes/plane.obj");
		wall->setShaderOne("res/shaders/lightingPassOne-vertex.glsl", "res/shaders/lightingPassOne-fragment.glsl");
		wall->setShaderTwo("res/shaders/lightingPassTwo-vertex.glsl", "res/shaders/lightingPassTwo-fragment.glsl");
		wall->setDiffuseTexture("res/textures/brick_diff.png");
		wall->setSpecularTexture("res/textures/brick_spec.png");
		m_sceneMeshes.push_back(wall);
	}
	
	//Roof
	std::vector<glm::vec3> RoofPosRot =
	{
		glm::vec3(0.0f, 6.0f, 0.0f), glm::vec3(0.0f, 0.0f, 180.0f),
		glm::vec3(6.0f, 6.0f, 0.0f), glm::vec3(0.0f, 0.0f, 180.0f),
		glm::vec3(-6.0f, 6.0f, 0.0f), glm::vec3(0.0f, 0.0f, 180.0f),
		glm::vec3(0.0f, 6.0f, 6.0f), glm::vec3(0.0f, 0.0f, 180.0f),
		glm::vec3(6.0f, 6.0f, 6.0f), glm::vec3(0.0f, 0.0f, 180.0f),
		glm::vec3(-6.0f, 6.0f, 6.0f), glm::vec3(0.0f, 0.0f, 180.0f),
		glm::vec3(0.0f, 6.0f, -6.0f), glm::vec3(0.0f, 0.0f, 180.0f),
		glm::vec3(6.0f, 6.0f, -6.0f), glm::vec3(0.0f, 0.0f, 180.0f),
		glm::vec3(-6.0f, 6.0f, -6.0f), glm::vec3(0.0f, 0.0f, 180.0f)
	};
	
	for (int i = 0; i < RoofPosRot.size(); i += 2)
	{
		Model* Floor = new Model(RoofPosRot.at(i), RoofPosRot.at(i + 1));
		Floor->setMesh("res/meshes/plane.obj");
		Floor->setShaderOne("res/shaders/lightingPassOne-vertex.glsl", "res/shaders/lightingPassOne-fragment.glsl");
		Floor->setShaderTwo("res/shaders/lightingPassTwo-vertex.glsl", "res/shaders/lightingPassTwo-fragment.glsl");
		Floor->setDiffuseTexture("res/textures/metal_diff.png");
		Floor->setSpecularTexture("res/textures/metal_spec.png");
		m_sceneMeshes.push_back(Floor);
	}

	//Crates
	std::vector<glm::vec3> CratePosRot =
	{
		glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-3.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f),
	};

	for (int i = 0; i < CratePosRot.size(); i += 2)
	{
		Model* Crate = new Model(CratePosRot.at(i), CratePosRot.at(i + 1));
		Crate->setMesh("res/meshes/crate.obj");
		Crate->setShaderOne("res/shaders/lightingPassOne-vertex.glsl", "res/shaders/lightingPassOne-fragment.glsl");
		Crate->setShaderTwo("res/shaders/lightingPassTwo-vertex.glsl", "res/shaders/lightingPassTwo-fragment.glsl");
		Crate->setDiffuseTexture("res/textures/crate_diff.png");
		Crate->setSpecularTexture("res/textures/crate_spec.png");
		m_sceneMeshes.push_back(Crate);
	}


	//Light
	//std::vector<glm::vec3> LightPos =
	//{
	//	//glm::vec3(6.0f, 3.0f, -6.0f),
	//	//glm::vec3(-6.0f, 3.0f, 6.0f)
	//};
	//
	//for (int i = 0; i < LightPos.size(); i++)
	//{
	//	Model* light = new Model(LightPos.at(i));
	//	light->setMesh("res/meshes/cube.obj");
	//	light->setShader("res/shaders/basic-vertex.glsl", "res/shaders/basic-fragment.glsl");
	//	light->SetXScale(0.3);
	//	light->SetYScale(0.3);
	//	light->SetZScale(0.3);
	//	m_sceneMeshes.push_back(light);
	//	m_sceneLightManager->addPointLight(LightPos.at(i).x, LightPos.at(i).y, LightPos.at(i).z);
	//}

	//Light
	std::vector<glm::vec3> LightPos =
	{
		glm::vec3(0.0f, 2.0f, -5.0f),
		//glm::vec3(-5.0f, 3.0f, 5.0f)
	};
	
	for (int i = 0; i < LightPos.size(); i++)
	{
		m_sceneLightManager->addPointLight(LightPos.at(i).x, LightPos.at(i).y, LightPos.at(i).z);
	}



}

void Scene::updateScene()
{
	m_sceneCamera->Update(0.025);



	lightVmatrix = glm::lookAt(m_sceneLightManager->getPointLight(0)->Position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	lightPmatrix = glm::perspective(glm::radians(60.0f), (float)1280 / (float)720, 0.1f, 1000.0f);

	//Bind the shadow framebuffer and associate it with the shadow texture
	glBindFramebuffer(GL_FRAMEBUFFER, shadowBuffer);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowTex, 0);

	//Disable drawing colours, but enable the depth computation
	glDrawBuffer(GL_NONE);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(5.0f, 4.0f);

	for (Model* m : m_sceneMeshes)
	{
		m->drawPassOne(lightVmatrix, lightPmatrix, shadowMVP1);
	}

	glDisable(GL_POLYGON_OFFSET_FILL);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glDrawBuffer(GL_FRONT);

	for (Model* m : m_sceneMeshes)
	{
		m->drawPassTwo(shadowMVP2, b, lightPmatrix, lightVmatrix);

	}
}

void Scene::setupShadowObjects()
{
	b = glm::mat4(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f
	);

	glfwGetFramebufferSize(EngineStatics::getAppWindow(), &screenSizeX, &screenSizeY);

	//Create custom frame buffer
	glGenFramebuffers(1, &shadowBuffer);

	//Create shadow texture and configure to hold depth information
	glGenTextures(1, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, screenSizeX, screenSizeY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Scene::addSceneCamera(float x, float y, float z)
{
	if (m_sceneCamera != nullptr)
	{
		delete m_sceneCamera;
		m_sceneCamera = nullptr;
	}
	
	m_sceneCamera = new Camera(glm::vec3{ x,y,z });
	EngineStatics::setCamera(m_sceneCamera);
}

void Scene::addSceneLightManager()
{
	if (m_sceneLightManager != nullptr)
	{
		delete m_sceneLightManager;
		m_sceneLightManager = nullptr;
	}

	m_sceneLightManager = new LightManager();
	EngineStatics::setLightManager(m_sceneLightManager);
}
