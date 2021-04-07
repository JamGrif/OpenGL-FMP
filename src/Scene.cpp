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
	m_sceneCamera = new Camera({0.0f, 3.0f, 0.0f});
	m_sceneLightManager = new LightManager();
	EngineStatics::setCamera(m_sceneCamera);
	EngineStatics::setLightManager(m_sceneLightManager);
	//m_sceneLightManager->addPointLight(-10.0f, 2.0f, 0.0f);
	//m_sceneLightManager->addPointLight(20.0f, 2.0f, 10.0f);
	//m_sceneLightManager->addPointLight(-15.0f, 2.0f, -10.0f);
	//m_sceneLightManager->addPointLight(4.0f, 2.0f, -20.0f);
	//m_sceneLightManager->addSpotLight(0.0f, 0.0f, 0.0f);

	m_sceneLightManager->addDirectionalLight(0.0f, -1.0f, -10.0f);


	for (int i = -5; i < 5; i+=1)
	{
		for (int j = -5; j < 5; j += 1)
		{
			Model* test = new Model();
			test->setMesh("res/meshes/plane.obj");
			test->setDiffuseTexture("res/textures/grass_diff.png");
			test->setSpecularTexture("res/textures/grass_spec.png");
			test->SetXPos(i * 12);
			test->SetZPos(j * 12);
			test->SetYPos(0.1);

			m_sceneMeshes.push_back(test);
		}
	}


	
	
	Model* test = new Model();
	test->setMesh("res/meshes/cottage.obj");
	test->setDiffuseTexture("res/textures/cottage_diff.png");
	test->setSpecularTexture("res/textures/cottage_spec.png");
	test->SetZPos(-10);
	m_sceneMeshes.push_back(test);
	
	Model* test2 = new Model();
	test2->setMesh("res/meshes/shack.obj");
	test2->setDiffuseTexture("res/textures/shack_diff.png");
	test2->setSpecularTexture("res/textures/shack_spec.png");
	test2->SetXPos(4.5);
	test2->SetYPos(0.5);
	test2->SetZPos(-10);
	m_sceneMeshes.push_back(test2);
	
	Model* test3 = new Model();
	test3->setMesh("res/meshes/cottage.obj");
	test3->setDiffuseTexture("res/textures/cottage_diff.png");
	test3->setSpecularTexture("res/textures/cottage_spec.png");
	test3->SetZPos(10);
	test3->IncYRot(180);
	m_sceneMeshes.push_back(test3);
	
	Model* test4 = new Model();
	test4->setMesh("res/meshes/shack.obj");
	test4->setDiffuseTexture("res/textures/shack_diff.png");
	test4->setSpecularTexture("res/textures/shack_spec.png");
	test4->SetXPos(-4.5);
	test4->SetYPos(0.5);
	test4->SetZPos(10);
	test4->IncYRot(180);
	m_sceneMeshes.push_back(test4);

}

void Scene::updateScene()
{
	m_sceneCamera->Update(0.025);

	for (Model* m : m_sceneMeshes)
	{
		m->drawModel();
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
