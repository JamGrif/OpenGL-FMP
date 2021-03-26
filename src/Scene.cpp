#include "Scene.h"

Scene::Scene()
	:m_sceneCamera(nullptr), m_sceneLightManager(nullptr)
{
	std::cout << "Scene Initialized" << std::endl;
}

Scene::~Scene()
{
	std::cout << "Scene Destroyed" << std::endl;
	
	for (Mesh* m : m_sceneMeshes)
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
	m_sceneCamera = new Camera({0.0f,2.0f,0.0f});
	m_sceneLightManager = new LightManager();
	EngineStatics::setCamera(m_sceneCamera);
	EngineStatics::setLightManager(m_sceneLightManager);
	m_sceneLightManager->addPointLight(-3.0f, 0.0f, 0.0f);


	for (int i = 0; i < 25; i+=5)
	{
		for (int j = 0; j < 25; j += 5)
		{
			Mesh* test = new Mesh("res/meshes/Cube.obj");
			test->SetXPos(-i*2.5);
			test->SetZPos(-j*2.5);
			test->IncXRot(i * 2);
			test->IncYRot(j * 2);
			test->IncZRot((i+j) * 2);
			m_sceneMeshes.push_back(test);
		}
	}

	m_sceneMeshes.push_back(cube = new Mesh("res/meshes/Cube.obj"));
	cube->SetYPos(-3);
	cube->SetXPos(-3);
	//cube->SetXScale(2);
	//cube->SetZScale(2);
	//cube->SetYScale(2);

}

void Scene::updateScene()
{
	m_sceneCamera->Update(0.025);
	//m_sceneLightManager->setPointLight(-0.1,-0.1);

	for (Mesh* m : m_sceneMeshes)
	{
		m->updateMesh();
	}
}

void Scene::addSceneCamera()
{
	if (m_sceneCamera != nullptr)
	{
		delete m_sceneCamera;
		m_sceneCamera = nullptr;
	}
	
	m_sceneCamera = new Camera({ 0.0f,0.0f,8.0f });
	EngineStatics::setCamera(m_sceneCamera);
}
