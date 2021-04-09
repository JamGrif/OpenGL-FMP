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
	addSceneCamera(0.0f, 3.0f, 0.0f);
	addSceneLightManager();
	
	m_sceneLightManager->addDirectionalLight(0.0f, -10.0f, -10.0f);
	m_sceneLightManager->addPointLight(-10.0f, 5.0f, 0.0f);
	m_sceneLightManager->addPointLight(20.0f, 5.0f, 10.0f);
	m_sceneLightManager->addPointLight(-15.0f, 5.0f, -10.0f);
	m_sceneLightManager->addPointLight(4.0f, 4.0f, -20.0f);
	 
	//m_sceneLightManager->addSpotLight(0.0f, 0.0f, 0.0f);

	

	//Floor
	for (int i = -5; i < 5; i+=1)
	{
		for (int j = -5; j < 5; j += 1)
		{
			Model* Floor = new Model();
			Floor->setMesh("res/meshes/plane.obj");
			Floor->setDiffuseTexture("res/textures/grass_diff.png");
			Floor->setSpecularTexture("res/textures/grass_spec.png");
			Floor->SetXPos(i * 12);
			Floor->SetZPos(j * 12);
			Floor->SetYPos(0.1);
			m_sceneMeshes.push_back(Floor);
		}
	}

	//Cottage
	for (int i = 0; i < CottagePosRot.size(); i += 2)
	{
		Model* cottage = new Model(CottagePosRot.at(i), CottagePosRot.at(i+1));
		cottage->setMesh("res/meshes/cottage.obj");
		cottage->setDiffuseTexture("res/textures/cottage_diff.png");
		cottage->setSpecularTexture("res/textures/cottage_spec.png");
		m_sceneMeshes.push_back(cottage);
	}


	//Shack
	for (int i = 0; i < ShackPosRot.size(); i += 2)
	{
		Model* shack = new Model(ShackPosRot.at(i), ShackPosRot.at(i + 1));
		shack->setMesh("res/meshes/shack.obj");
		shack->setDiffuseTexture("res/textures/shack_diff.png");
		shack->setSpecularTexture("res/textures/shack_spec.png");
		m_sceneMeshes.push_back(shack);
	}

}

void Scene::updateScene()
{
	m_sceneCamera->Update(0.025);

	for (Model* m : m_sceneMeshes)
	{
		m->drawModel();
	}
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
