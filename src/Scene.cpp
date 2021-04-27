#include "Scene.h"

#include "EngineStatics.h"

Scene::Scene()
	:m_sceneCamera(nullptr), m_sceneLightManager(nullptr)
{
	std::cout << "Scene Initialized" << std::endl;
}

Scene::~Scene()
{
	
	
	for (Model* m : m_sceneMeshes)
	{
		delete m;
	}
	m_sceneMeshes.clear();

	EngineStatics::setCamera(nullptr);
	delete m_sceneCamera;
	m_sceneCamera = nullptr;

	EngineStatics::setLightManager(nullptr);
	delete m_sceneLightManager;
	m_sceneLightManager = nullptr;

	std::cout << "Scene Destroyed" << std::endl;
}

/// <summary>
/// Initalizes the scene objects and creates the scenes models
/// </summary>
void Scene::initScene()
{
	setupShadowObjects();
	
	addSceneCamera(0.0f, 2.0f, 0.0f);
	addSceneLightManager();
	
	//m_sceneLightManager->addDirectionalLight(0.0f, -5.0f, -10.0f);
	
	//m_sceneLightManager->addSpotLight(0.0f, 0.0f, 0.0f);

	//Floor
	//std::vector<glm::vec3> FloorPosRot =
	//{
	//	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
	//	glm::vec3(6.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
	//	glm::vec3(-6.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
	//	glm::vec3(0.0f, 0.0f, 6.0f), glm::vec3(0.0f, 0.0f, 0.0f),
	//	glm::vec3(6.0f, 0.0f, 6.0f), glm::vec3(0.0f, 0.0f, 0.0f),
	//	glm::vec3(-6.0f, 0.0f, 6.0f), glm::vec3(0.0f, 0.0f, 0.0f),
	//	glm::vec3(0.0f, 0.0f, -6.0f), glm::vec3(0.0f, 0.0f, 0.0f),
	//	glm::vec3(6.0f, 0.0f, -6.0f), glm::vec3(0.0f, 0.0f, 0.0f),
	//	glm::vec3(-6.0f, 0.0f, -6.0f), glm::vec3(0.0f, 0.0f, 0.0f)
	//};
	//
	//
	//for (int i = 0; i < FloorPosRot.size(); i += 2)
	//{
	//	ModelLighting* Floor = new ModelLighting(FloorPosRot.at(i), FloorPosRot.at(i + 1));
	//	Floor->setMesh("res/meshes/plane.obj");
	//	Floor->setDiffuseTexture("res/textures/concrete_diff.png");
	//	Floor->setSpecularTexture("res/textures/concrete_spec.png");
	//	Floor->setNormalTexture("res/textures/concrete_norm.png", true);
	//	//Floor->setEmissionTexture("res/textures/matrix_emis.png");
	//	m_sceneMeshes.push_back(Floor);
	//}

	
	
	//Z wall
	std::vector<glm::vec3> WallPosRot =
	{
		glm::vec3(-6.0f, 3.0f, -9.0f), glm::vec3(90.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 3.0f, -9.0f), glm::vec3(90.0f, 0.0f, 0.0f),
		glm::vec3(6.0f, 3.0f, -9.0f), glm::vec3(90.0f, 0.0f, 0.0f),
		glm::vec3(-6.0f, 3.0f, 9.0f), glm::vec3(-90.0f, 180.0f, 0.0f),
		glm::vec3(0.0f, 3.0f, 9.0f), glm::vec3(-90.0f, 180.0f, 0.0f),
		glm::vec3(6.0f, 3.0f, 9.0f), glm::vec3(-90.0f, 180.0f, 0.0f)
	
	};
	
	for (int i = 0; i < WallPosRot.size(); i += 2)
	{
		ModelLighting* wall = new ModelLighting(WallPosRot.at(i), WallPosRot.at(i+1));
		wall->setMesh("res/meshes/plane.obj");
		wall->setDiffuseTexture("res/textures/cartoonBrick_diff.png");
		wall->setSpecularTexture("res/textures/cartoonBrick_spec.png");
		wall->setNormalTexture("res/textures/cartoonBrick_norm.png", false);
		wall->setHeightTexture("res/textures/cartoonBrick_height.png");
		//wall->setEmissionTexture("res/textures/cartoonBrick_emis.png");
		m_sceneMeshes.push_back(wall);
	}

	//Side wall
	//std::vector<glm::vec3> SideWallPosRot =
	//{
	//	glm::vec3(-9.0f, 3.0f, -6.0f), glm::vec3(90.0f, 0.0f, -90.0f),
	//	glm::vec3(-9.0f, 3.0f, 0.0f), glm::vec3(90.0f, 0.0f, -90.0f),
	//	glm::vec3(-9.0f, 3.0f, 6.0f), glm::vec3(90.0f, 0.0f, -90.0f),
	//	glm::vec3(9.0f, 3.0f, -6.0f), glm::vec3(90.0f, 0.0f, 90.0f),
	//	glm::vec3(9.0f, 3.0f, 0.0f), glm::vec3(90.0f, 0.0f, 90.0f),
	//	glm::vec3(9.0f, 3.0f, 6.0f), glm::vec3(90.0f, 0.0f, 90.0f)
	//};
	//
	//for (int i = 0; i < SideWallPosRot.size(); i += 2)
	//{
	//	ModelLighting* wall = new ModelLighting(SideWallPosRot.at(i), SideWallPosRot.at(i + 1));
	//	wall->setMesh("res/meshes/plane.obj");
	//	wall->setDiffuseTexture("res/textures/wood2_diff.png");
	//	wall->setSpecularTexture("res/textures/wood2_spec.png");
	//	wall->setNormalTexture("res/textures/wood2_norm.png", false);
	//	m_sceneMeshes.push_back(wall);
	//}
	//
	////Roof
	//std::vector<glm::vec3> RoofPosRot =
	//{
	//	glm::vec3(0.0f, 6.0f, 0.0f), glm::vec3(0.0f, 0.0f, 180.0f),
	//	glm::vec3(6.0f, 6.0f, 0.0f), glm::vec3(0.0f, 0.0f, 180.0f),
	//	glm::vec3(-6.0f, 6.0f, 0.0f), glm::vec3(0.0f, 0.0f, 180.0f),
	//	glm::vec3(0.0f, 6.0f, 6.0f), glm::vec3(0.0f, 0.0f, 180.0f),
	//	glm::vec3(6.0f, 6.0f, 6.0f), glm::vec3(0.0f, 0.0f, 180.0f),
	//	glm::vec3(-6.0f, 6.0f, 6.0f), glm::vec3(0.0f, 0.0f, 180.0f),
	//	glm::vec3(0.0f, 6.0f, -6.0f), glm::vec3(0.0f, 0.0f, 180.0f),
	//	glm::vec3(6.0f, 6.0f, -6.0f), glm::vec3(0.0f, 0.0f, 180.0f),
	//	glm::vec3(-6.0f, 6.0f, -6.0f), glm::vec3(0.0f, 0.0f, 180.0f)
	//};
	//
	//for (int i = 0; i < RoofPosRot.size(); i += 2)
	//{
	//	ModelLighting* Floor = new ModelLighting(RoofPosRot.at(i), RoofPosRot.at(i + 1));
	//	Floor->setMesh("res/meshes/plane.obj");
	//	Floor->setDiffuseTexture("res/textures/metal2_diff.png");
	//	Floor->setSpecularTexture("res/textures/metal2_spec.png");
	//	Floor->setNormalTexture("res/textures/metal2_norm.png", false);
	//	m_sceneMeshes.push_back(Floor);
	//}
	//
	////Crates
	//std::vector<glm::vec3> CratePosRot =
	//{
	//	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
	//	glm::vec3(-3.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f),
	//};
	//
	//for (int i = 0; i < CratePosRot.size(); i += 2)
	//{
	//	ModelLighting* Crate = new ModelLighting(CratePosRot.at(i), CratePosRot.at(i + 1));
	//	Crate->setMesh("res/meshes/crate.obj");
	//	Crate->setDiffuseTexture("res/textures/crate_diff.png");
	//	Crate->setSpecularTexture("res/textures/crate_spec.png");
	//	Crate->setNormalTexture("res/textures/crate_norm.png", false);
	//	m_sceneMeshes.push_back(Crate);
	//}


	//Light
	std::vector<glm::vec3> LightPos =
	{
		glm::vec3(0.0f, 3.0f, 0.0f),
		//glm::vec3(-3.0f, 3.0f, -5.0f),
		//glm::vec3(-3.0f, 3.0f, 5.0f),
		//glm::vec3(3.0f, 3.0f, 5.0f)
	};
	
	for (int i = 0; i < LightPos.size(); i++)
	{
		Model* light = new ModelBasic(LightPos.at(i));
		light->setMesh("res/meshes/cube.obj");
		light->SetXScale(0.3);
		light->SetYScale(0.3);
		light->SetZScale(0.3);
		m_sceneMeshes.push_back(light);
	
		m_sceneLightManager->addPointLight(LightPos.at(i).x, LightPos.at(i).y, LightPos.at(i).z);
	}



}

/// <summary>
/// Updates scene objects and models
/// </summary>
void Scene::updateScene()
{
	m_sceneCamera->Update(0.025);


	for (Model* m : m_sceneMeshes)
	{
		m->drawPassOne();
	}


	for (Model* m : m_sceneMeshes)
	{
		m->drawPassTwo();

	}
}

void Scene::setupShadowObjects()
{
	
}

/// <summary>
/// Creates the scene camera, remakes it if one already exists
/// </summary>
/// <param name="x">Starting X position of camera</param>
/// <param name="y">Starting Y position of camera</param>
/// <param name="z">Starting Z position of camera</param>
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

/// <summary>
/// Creates the scene light mananger, remakes it if one already exists
/// </summary>
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
