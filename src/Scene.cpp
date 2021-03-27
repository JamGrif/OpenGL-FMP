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


	for (int i = 0; i < 10; i+=5)
	{
		for (int j = 0; j < 10; j += 5)
		{
			Mesh* test = new Mesh("res/meshes/cottage.obj");
			test->loadDiffuseTexture("res/textures/cottage_diff.jpg");
			test->loadSpecularTexture("res/textures/cottage_spec.jpg");
			test->SetXPos(-i * 4.5);
			test->SetZPos(10 + (-j * 4.5));
			//test->SetZPos(-10);
			test->SetXScale(0.4);
			test->SetYScale(0.4);
			test->SetZScale(0.4);
			m_sceneMeshes.push_back(test);
		}
	}

	//Mesh* test = new Mesh("res/meshes/cottage.obj");
	//test->loadDiffuseTexture("res/textures/cottage_diff.jpg");
	//test->loadSpecularTexture("res/textures/cottage_spec.jpg");
	//test->SetXPos(0);
	//test->SetYPos(-4);
	//test->SetZPos(-10);
	//test->SetXScale(0.4);
	//test->SetYScale(0.4);
	//test->SetZScale(0.4);
	//m_sceneMeshes.push_back(test);
	//
	//Mesh* test2 = new Mesh("res/meshes/cottage.obj");
	//test2->loadDiffuseTexture("res/textures/cottage_diff.jpg");
	//test2->loadSpecularTexture("res/textures/cottage_spec.jpg");
	//test2->SetXPos(0);
	//test2->SetYPos(-4);
	//test2->SetZPos(10);
	//test2->SetXScale(0.4);
	//test2->SetYScale(0.4);
	//test2->SetZScale(0.4);
	//m_sceneMeshes.push_back(test2);

	//m_sceneMeshes.push_back(cube = new Mesh("res/meshes/Cube.obj"));
	//cube->SetYPos(-3);
	//cube->SetXPos(-3);
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
		//m->IncXRot(1);
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
