#include "Scene.h"

#include "EngineStatics.h"

#include "Input.h"

Scene::Scene()
	:m_sceneCamera(nullptr), m_sceneLightManager(nullptr), m_sceneMSAAFrameBuffer(nullptr), m_sceneFilterFramebuffer(nullptr)
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

	TextureManager::clearTextures();
	TextureManager::clearCubemaps();
	MeshManager::clearMeshes();
	ShaderManager::clearShaders();

	std::cout << "Scene Destroyed" << std::endl;
}

/// <summary>
/// Initalizes the scene objects and creates the scenes models
/// </summary>
void Scene::initScene()
{

	//setupSceneFramebuffer();

	//setupShadowStuff();
	
	addSceneCamera(0.0f, 2.0f, 0.0f);
	addSceneLightManager();

	//Skybox
	m_sceneMeshes.push_back(new ModelSky());

	m_sceneFilterFramebuffer = new Framebuffer(false);
	m_sceneMSAAFrameBuffer = new Framebuffer(true);

	//Terrain
	m_sceneMeshes.push_back(new Terrain(glm::vec3(0.0f, 22.0f, 0.0f), glm::vec3(180.0f, 0.0f, 0.0f), glm::vec3(150,10,150), 3.0f)); //Surrounding house

	m_sceneMeshes.push_back(m_mountainsX = new Terrain(glm::vec3(125.0f, 20.0f, -30.0f), glm::vec3(180.0f, 0.0f, 0.0f), glm::vec3(50,10,200), -4.5f)); //Mountain on X axis
	m_sceneMeshes.push_back(m_mountainsZ = new Terrain(glm::vec3(30.0f, 20.0f, -125.0f), glm::vec3(180.0f, 90.0f, 0.0f), glm::vec3(50, 10, 200), -4.5f)); //Mountain on Z axis

	//Geometry shader object
	ModelGeometry* g = new ModelGeometry(glm::vec3(-1.0f, -1.5f, 23.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	g->setMesh("res/meshes/barrel.obj");
	g->SetXScale(1.5);
	g->SetYScale(1.5);
	g->SetZScale(1.5);
	m_sceneMeshes.push_back(g);
	
	ModelEnvironment* reflectionModel = new ModelEnvironment(glm::vec3(14.0f, 0.5f, -7.0f));
	reflectionModel->toggleReflection(true);
	reflectionModel->setMesh("res/meshes/heart.obj");
	reflectionModel->SetXScale(1);
	reflectionModel->SetYScale(1);
	reflectionModel->SetZScale(1);
	m_sceneMeshes.push_back(reflectionModel);
	
	ModelEnvironment* refractionModel = new ModelEnvironment(glm::vec3(22.0f, 0.5f, -7.0f));
	refractionModel->toggleRefraction(true);
	refractionModel->setMesh("res/meshes/heart.obj");
	refractionModel->SetXScale(1);
	refractionModel->SetYScale(1);
	refractionModel->SetZScale(1);
	m_sceneMeshes.push_back(refractionModel);

	//std::vector<glm::vec3> grassPos =
	//{
	//	glm::vec3(2.0f, 0.5f, 2.0f),
	//	glm::vec3(-2.0f, 0.5f, -2.0f),
	//	glm::vec3(2.0f, 0.5f, -2.0f),
	//	glm::vec3(-2.0f, 0.5f, 2.0f),
	//	glm::vec3(1.0f, 0.5f, 5.0f),
	//};
	//
	//for (int i = 0; i < grassPos.size(); i++)
	//{
	//	ModelSprite* grass = new ModelSprite(grassPos.at(i), glm::vec3(0.0f, 0.0f, 0.0f));
	//	grass->setSprite("res/textures/grass_sprite.png");
	//	grass->SetXScale(0.3);
	//	grass->SetYScale(0.3);
	//	grass->SetZScale(0.3);
	//	m_sceneMeshes.push_back(grass);
	//}

	

	
	m_sceneLightManager->addDirectionalLight(-2.0f, -3.0f, -1.0f, glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.3f, 0.3f, 0.3f));
	
	m_sceneLightManager->addSpotLight(0.0f, 0.0f, 0.0f, glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.8f,0.8f,0.8f), glm::vec3(1.0f, 1.0f, 1.0f));
	m_sceneLightManager->getSpotLight(0)->toggleActive(); //Turns off spotlight by default

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
		ModelLighting* Floor = new ModelLighting(FloorPosRot.at(i), FloorPosRot.at(i + 1));
		Floor->setMesh("res/meshes/plane.obj");
		Floor->setDiffuseTexture("res/textures/carpet_diff.png");
		Floor->setSpecularTexture("res/textures/carpet_spec.png");
		Floor->setNormalTexture("res/textures/carpet_norm.png", false);
		Floor->setHeightTexture("res/textures/carpet_height.png", 0.05f);
		//Floor->setEmissionTexture("res/textures/matrix_emis.png");
		m_sceneMeshes.push_back(Floor);
	}
	
	std::vector<glm::vec3> ConcreteFloor =
	{
		glm::vec3(0.0f, -0.8f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(6.0f, -0.8f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-6.0f, -0.8f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, -0.8f, 6.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(6.0f, -0.8f, 6.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-6.0f, -0.8f, 6.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, -0.8f, -6.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(6.0f, -0.8f, -6.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-6.0f, -0.8f, -6.0f), glm::vec3(0.0f, 0.0f, 0.0f)
	};
	
	
	for (int i = 0; i < ConcreteFloor.size(); i += 2)
	{
		ModelLighting* Floor = new ModelLighting(ConcreteFloor.at(i), ConcreteFloor.at(i + 1));
		Floor->setMesh("res/meshes/slab.obj");
		Floor->setDiffuseTexture("res/textures/concreteSlab_diff.png");
		Floor->setSpecularTexture("res/textures/concreteSlab_spec.png");
		Floor->setNormalTexture("res/textures/concreteSlab_norm.png", false);
		Floor->setHeightTexture("res/textures/concreteSlab_height.png", 0.01f);
		m_sceneMeshes.push_back(Floor);
	}
	
	//Side wall
	std::vector<glm::vec3> SideWallPosRot =
	{
		glm::vec3(-9.0f, 2.8f, -6.0f), glm::vec3(90.0f, 0.0f, -90.0f),
		glm::vec3(-9.0f, 2.8f, 0.0f), glm::vec3(90.0f, 0.0f, -90.0f),
		glm::vec3(-9.0f, 2.8f, 6.0f), glm::vec3(90.0f, 0.0f, -90.0f),
		glm::vec3(9.0f, 2.8f, -6.0f), glm::vec3(90.0f, 0.0f, 90.0f),
		glm::vec3(9.0f, 2.8f, 0.0f), glm::vec3(90.0f, 0.0f, 90.0f),
		glm::vec3(9.0f, 2.8f, 6.0f), glm::vec3(90.0f, 0.0f, 90.0f),
		glm::vec3(-6.0f, 2.8f, -9.0f), glm::vec3(90.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 2.8f, -9.0f), glm::vec3(90.0f, 0.0f, 0.0f),
		glm::vec3(6.0f, 2.8f, -9.0f), glm::vec3(90.0f, 0.0f, 0.0f),
		glm::vec3(-6.0f, 2.8f, 9.0f), glm::vec3(-90.0f, 180.0f, 0.0f),
		glm::vec3(6.0f, 2.8f, 9.0f), glm::vec3(-90.0f, 180.0f, 0.0f)
	};
	
	for (int i = 0; i < SideWallPosRot.size(); i += 2)
	{
		ModelLighting* wall = new ModelLighting(SideWallPosRot.at(i), SideWallPosRot.at(i + 1));
		wall->setMesh("res/meshes/wall.obj");
		wall->setDiffuseTexture("res/textures/woodWall_diff.png");
		wall->setSpecularTexture("res/textures/woodWall_spec.png");
		wall->setNormalTexture("res/textures/woodWall_norm.png", false);
		wall->setHeightTexture("res/textures/woodWall_height.png", 0.04);
		m_sceneMeshes.push_back(wall);
	}
	
	////Roof
	std::vector<glm::vec3> RoofPosRot =
	{
		glm::vec3(0.0f, 5.8f, 0.0f), glm::vec3(0.0f, 0.0f, 180.0f),
		glm::vec3(6.0f, 5.8f, 0.0f), glm::vec3(0.0f, 0.0f, 180.0f),
		glm::vec3(-6.0f, 5.8f, 0.0f), glm::vec3(0.0f, 0.0f, 180.0f),
		glm::vec3(0.0f, 5.8f, 6.0f), glm::vec3(0.0f, 0.0f, 180.0f),
		glm::vec3(6.0f, 5.8f, 6.0f), glm::vec3(0.0f, 0.0f, 180.0f),
		glm::vec3(-6.0f, 5.8f, 6.0f), glm::vec3(0.0f, 0.0f, 180.0f),
		glm::vec3(0.0f, 5.8f, -6.0f), glm::vec3(0.0f, 0.0f, 180.0f),
		glm::vec3(6.0f, 5.8f, -6.0f), glm::vec3(0.0f, 0.0f, 180.0f),
		glm::vec3(-6.0f, 5.8f, -6.0f), glm::vec3(0.0f, 0.0f, 180.0f)
	};
	
	for (int i = 0; i < RoofPosRot.size(); i += 2)
	{
		ModelLighting* Floor = new ModelLighting(RoofPosRot.at(i), RoofPosRot.at(i + 1));
		Floor->setMesh("res/meshes/wall.obj");
		Floor->setDiffuseTexture("res/textures/metal2_diff.png");
		Floor->setSpecularTexture("res/textures/metal2_spec.png");
		Floor->setNormalTexture("res/textures/metal2_norm.png", false);
		m_sceneMeshes.push_back(Floor);
	}
	
	//Dresser
	std::vector<glm::vec3> DresserPosRot =
	{
		glm::vec3(0.0f, 0.0f, -7.75f), glm::vec3(0.0f, 0.0f, 0.0f),
	};
	
	for (int i = 0; i < DresserPosRot.size(); i += 2)
	{
		ModelLighting* dresser = new ModelLighting(DresserPosRot.at(i), DresserPosRot.at(i + 1));
		dresser->setMesh("res/meshes/dresser.obj");
		dresser->setDiffuseTexture("res/textures/dresser_diff.png");
		dresser->setSpecularTexture("res/textures/dresser_spec.png");
		dresser->setNormalTexture("res/textures/dresser_norm.png", false);
		m_sceneMeshes.push_back(dresser);
	}
	
	//Front Stair
	std::vector<glm::vec3> StairsPos =
	{
		glm::vec3(0.0f, -2.25f, 10.0f), glm::vec3(0.0f, 90.0f, 0.0f),
	};
	
	for (int i = 0; i < StairsPos.size(); i += 2)
	{
		ModelLighting* Crate = new ModelLighting(StairsPos.at(i), StairsPos.at(i + 1));
		Crate->setMesh("res/meshes/stairs.obj");
		Crate->setDiffuseTexture("res/textures/wood2_diff.png");
		Crate->setSpecularTexture("res/textures/wood2_spec.png");
		Crate->setNormalTexture("res/textures/wood2_norm.png", false);
		m_sceneMeshes.push_back(Crate);
	}
	
	//Grass
	std::vector<glm::vec3> GrassPosRot =
	{
		glm::vec3(0.0f, -1.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(6.0f, -1.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-6.0f,-1.5f,  0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, -1.5f, 6.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(6.0f, -1.5f, 6.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-6.0f,-1.5f,  6.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, -1.5f, -6.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(6.0f, -1.5f, -6.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-6.0f,-1.5f,  -6.0f), glm::vec3(0.0f, 0.0f, 0.0f),
	
		glm::vec3(-18.0f, -1.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-12.0f, -1.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-24.0f, -1.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-18.0f, -1.5f, 6.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-12.0f, -1.5f, 6.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-24.0f, -1.5f, 6.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-18.0f, -1.5f, -6.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-12.0f, -1.5f, -6.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-24.0f, -1.5f, -6.0f), glm::vec3(0.0f, 0.0f, 0.0f),
	
		glm::vec3(18.0f, -1.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(12.0f, -1.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(24.0f, -1.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(18.0f, -1.5f, 6.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(12.0f, -1.5f, 6.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(24.0f, -1.5f, 6.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(18.0f, -1.5f, -6.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(12.0f, -1.5f, -6.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(24.0f, -1.5f, -6.0f), glm::vec3(0.0f, 0.0f, 0.0f),
	
		//
		glm::vec3(0.0f, -1.5f, 18.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(6.0f, -1.5f, 18.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-6.0f,-1.5f,  18.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, -1.5f, 24.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(6.0f, -1.5f, 24.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-6.0f,-1.5f,  24.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, -1.5f, 12.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(6.0f, -1.5f, 12.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-6.0f,-1.5f,  12.0f), glm::vec3(0.0f, 0.0f, 0.0f),
	
		glm::vec3(-18.0f, -1.5f, 18.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-12.0f, -1.5f, 18.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-24.0f, -1.5f, 18.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-18.0f, -1.5f, 24.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-12.0f, -1.5f, 24.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-24.0f, -1.5f, 24.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-18.0f, -1.5f, 12.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-12.0f, -1.5f, 12.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-24.0f, -1.5f, 12.0f), glm::vec3(0.0f, 0.0f, 0.0f),
	
		glm::vec3(18.0f, -1.5f, 18.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(12.0f, -1.5f, 18.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(24.0f, -1.5f, 18.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(18.0f, -1.5f, 24.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(12.0f, -1.5f, 24.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(24.0f, -1.5f, 24.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(18.0f, -1.5f, 12.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(12.0f, -1.5f, 12.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(24.0f, -1.5f, 12.0f), glm::vec3(0.0f, 0.0f, 0.0f)
	};
	
	
	for (int i = 0; i < GrassPosRot.size(); i += 2)
	{
		ModelLighting* Grass = new ModelLighting(GrassPosRot.at(i), GrassPosRot.at(i + 1));
		Grass->setMesh("res/meshes/plane.obj");
		Grass->setDiffuseTexture("res/textures/grass_diff.png");
		Grass->setSpecularTexture("res/textures/grass_spec.png");
		m_sceneMeshes.push_back(Grass);
	}
	
	
	//Side wall
	std::vector<glm::vec3> OutsideWall =
	{
		glm::vec3(-12.0f, 1.5f, -9.0f), glm::vec3(90.0f, 0.0f, 0.0f),
		glm::vec3(-18.0f, 1.5f, -9.0f), glm::vec3(90.0f, 0.0f, 0.0f),
		glm::vec3(-24.0f, 1.5f, -9.0f), glm::vec3(90.0f, 0.0f, 0.0f),

		glm::vec3(-27.0f, 1.5f, -6.0f), glm::vec3(90.0f, 0.0f, -90.0f),
		glm::vec3(-27.0f, 1.5f, 0.0f), glm::vec3(90.0f, 0.0f, -90.0f),
		glm::vec3(-27.0f, 1.5f, 6.0f), glm::vec3(90.0f, 0.0f, -90.0f),
		glm::vec3(-27.0f, 1.5f, 12.0f), glm::vec3(90.0f, 0.0f, -90.0f),
		glm::vec3(-27.0f, 1.5f, 18.0f), glm::vec3(90.0f, 0.0f, -90.0f),
		glm::vec3(-27.0f, 1.5f, 24.0f), glm::vec3(90.0f, 0.0f, -90.0f),

		glm::vec3(-24.0f, 1.5f, 27.0f), glm::vec3(-90.0f, 0.0f, 0.0f),
		glm::vec3(-18.0f, 1.5f, 27.0f), glm::vec3(-90.0f, 0.0f, 0.0f),
		glm::vec3(-12.0f, 1.5f, 27.0f), glm::vec3(-90.0f, 0.0f, 0.0f),
		glm::vec3(-6.0f, 1.5f, 27.0f), glm::vec3(-90.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.5f, 27.0f), glm::vec3(-90.0f, 0.0f, 0.0f),
		glm::vec3(6.0f, 1.5f, 27.0f), glm::vec3(-90.0f, 0.0f, 0.0f),
		glm::vec3(12.0f, 1.5f, 27.0f), glm::vec3(-90.0f, 0.0f, 0.0f),
		glm::vec3(18.0f, 1.5f, 27.0f), glm::vec3(-90.0f, 0.0f, 0.0f),
		glm::vec3(24.0f, 1.5f, 27.0f), glm::vec3(-90.0f, 0.0f, 0.0f),

		glm::vec3(27.0f, 1.5f, 24.0f), glm::vec3(90.0f, 0.0f, 90.0f),
		glm::vec3(27.0f, 1.5f, 18.0f), glm::vec3(90.0f, 0.0f, 90.0f),
		glm::vec3(27.0f, 1.5f, 12.0f), glm::vec3(90.0f, 0.0f, 90.0f),
		glm::vec3(27.0f, 1.5f, 6.0f), glm::vec3(90.0f, 0.0f, 90.0f),
		glm::vec3(27.0f, 1.5f, 0.0f), glm::vec3(90.0f, 0.0f, 90.0f),
		glm::vec3(27.0f, 1.5f, -6.0f), glm::vec3(90.0f, 0.0f, 90.0f),

		glm::vec3(12.0f, 1.5f, -9.0f), glm::vec3(90.0f, 0.0f, 0.0f),
		glm::vec3(18.0f, 1.5f, -9.0f), glm::vec3(90.0f, 0.0f, 0.0f),
		glm::vec3(24.0f, 1.5f, -9.0f), glm::vec3(90.0f, 0.0f, 0.0f),
	};
	
	for (int i = 0; i < OutsideWall.size(); i += 2)
	{
		ModelLighting* wall = new ModelLighting(OutsideWall.at(i), OutsideWall.at(i + 1));
		wall->setMesh("res/meshes/plane.obj");
		wall->setDiffuseTexture("res/textures/concreteBrick_diff.png");
		wall->setSpecularTexture("res/textures/concreteBrick_spec.png");
		wall->setNormalTexture("res/textures/concreteBrick_norm.png", false);
		wall->setHeightTexture("res/textures/concreteBrick_height.png", 0.01);
		m_sceneMeshes.push_back(wall);
	}

	//Shed
	std::vector<glm::vec3> ShedPos =
	{
		glm::vec3(-38.0f, 1.0f, 0.0f), glm::vec3(0.0f, 210.0f, 0.0f)
	};

	for (int i = 0; i < ShedPos.size(); i += 2)
	{
		ModelLighting* dresser = new ModelLighting(ShedPos.at(i), ShedPos.at(i + 1));
		dresser->setMesh("res/meshes/shed.obj");
		dresser->setDiffuseTexture("res/textures/shed_diff.png");
		dresser->setSpecularTexture("res/textures/shed_spec.png");
		dresser->setNormalTexture("res/textures/shed_norm.png", false);
		m_sceneMeshes.push_back(dresser);
	}

	//Sign
	std::vector<glm::vec3> SignPosRot =
	{
		glm::vec3(1.0f, -3.5f, 25.75f), glm::vec3(0.0f, 180.0f, 0.0f),		//Breating barrel /
		glm::vec3(10.0f, 10.0f, -7.75f), glm::vec3(0.0f, 0.0f, 0.0f),		//Q and E tessellation shader
		glm::vec3(15.0f, 10.0f, -7.75f), glm::vec3(0.0f, 0.0f, 0.0f),		//Fake depth from height / normal
		glm::vec3(20.0f, 10.0f, -7.75f), glm::vec3(0.0f, 0.0f, 0.0f),		//Sign text from emission
		glm::vec3(11.0f, -3.5f, -4.75f), glm::vec3(0.0f, 30.0f, 0.0f),		//Environment reflect /
		glm::vec3(25.0f, -3.5f, -4.75f), glm::vec3(0.0f, -30.0f, 0.0f),		//Environment refract /
		glm::vec3(35.0f, 10.0f, -7.75f), glm::vec3(0.0f, 0.0f, 0.0f),		//Normal vs no normal
		glm::vec3(40.0f, 10.0f, -7.75f), glm::vec3(0.0f, 0.0f, 0.0f),		//Different materials
		glm::vec3(45.0f, 10.0f, -7.75f), glm::vec3(0.0f, 0.0f, 0.0f),		//Colour lighting
	};

	std::vector <const char*> SignTex =
	{
		"res/textures/signs/sign1_emis.png",
		"res/textures/signs/sign2_emis.png",
		"res/textures/signs/sign3_emis.png",
		"res/textures/signs/sign4_emis.png",
		"res/textures/signs/sign5_emis.png",
		"res/textures/signs/sign6_emis.png",
		"res/textures/signs/sign7_emis.png",
		"res/textures/signs/sign8_emis.png",
		"res/textures/signs/sign9_emis.png"
	};

	int signNum = 0;
	for (int i = 0; i < SignPosRot.size(); i += 2)
	{
		ModelLighting* dresser = new ModelLighting(SignPosRot.at(i), SignPosRot.at(i + 1));
		dresser->setMesh("res/meshes/signpost.obj");
		dresser->setDiffuseTexture("res/textures/sign_diff.png");
		dresser->setSpecularTexture("res/textures/sign_spec.png");
		dresser->setNormalTexture("res/textures/sign_norm.png", false);

		dresser->setEmissionTexture(SignTex.at(signNum));
		m_sceneMeshes.push_back(dresser);
		signNum++;
	}
	
	//Light
	std::vector<glm::vec3> LightPosAmbDifSpc =
	{
		glm::vec3(0.0f, 3.0f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.4f, 0.4f, 0.4f),
		glm::vec3(-15.0f, 3.0f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.4f, 0.4f, 0.4f),
		glm::vec3(15.0f, 3.0f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.4f, 0.4f, 0.4f),
	};
	
	for (int i = 0; i < LightPosAmbDifSpc.size(); i+=4)
	{
		m_sceneLightManager->addPointLight(LightPosAmbDifSpc.at(i).x, LightPosAmbDifSpc.at(i).y, LightPosAmbDifSpc.at(i).z, LightPosAmbDifSpc.at(i+1), LightPosAmbDifSpc.at(i+2), LightPosAmbDifSpc.at(i+3));
	}

	//Objects that copy lights
	for (int i = 0; i < m_sceneLightManager->getCurrentPointLights(); i++)
	{
		ModelBasic* light = new ModelBasic(LightPosAmbDifSpc.at(i));
		light->setMesh("res/meshes/cube.obj");
		light->copyPointLight(i);
		m_sceneMeshes.push_back(light);
	}

}

/// <summary>
/// Updates scene objects and models
/// </summary>
void Scene::updateScene()
{
	m_sceneCamera->Update(EngineStatics::getDeltaTime());

	updateOnInput();

	m_sceneMSAAFrameBuffer->bindFramebuffer();

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Draw first pass of all models
	//glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	//glClear(GL_DEPTH_BUFFER_BIT);

	


	for (Model* m : m_sceneMeshes)
	{
		m->setMatrixValues();
		//m->drawPassOne();
	}

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glActiveTexture(GL_TEXTURE5);
	//glBindTexture(GL_TEXTURE_2D, depthMap);


	//Draw second pass of all models
	for (Model* m : m_sceneMeshes)
	{
		m->drawPassTwo();
	}

	//Reads from the MSAA buffer and writes it to the Filter buffer
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_sceneMSAAFrameBuffer->getFBO());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_sceneFilterFramebuffer->getFBO());
	glBlitFramebuffer(0, 0, 1280, 720, 0, 0, 1280, 720, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	
	m_sceneMSAAFrameBuffer->unbindFramebuffer();
	
	m_sceneFilterFramebuffer->draw();

}

/// <summary>
/// Checks for input and updates various game objects as a result of those inputs - called every scene update
/// </summary>
void Scene::updateOnInput()
{
	/*
		Framebuffer filters inputs
	*/

	if (Input::getKeyPressed(GLFW_KEY_1))
	{
		m_sceneFilterFramebuffer->setFrameFilter(screen_Default);
	}

	if (Input::getKeyPressed(GLFW_KEY_2))
	{
		m_sceneFilterFramebuffer->setFrameFilter(screen_Inverse);
	}

	if (Input::getKeyPressed(GLFW_KEY_3))
	{
		m_sceneFilterFramebuffer->setFrameFilter(screen_Greyscale);
	}

	if (Input::getKeyPressed(GLFW_KEY_4))
	{
		m_sceneFilterFramebuffer->setFrameFilter(screen_EdgeDetection);
	}

	if (Input::getKeyPressed(GLFW_KEY_5))
	{
		m_sceneFilterFramebuffer->setFrameFilter(screen_Drugs);
	}

	/*
		Tessellation terrain inputs
	*/

	if (Input::getKeyPressedOnce(GLFW_KEY_Q))
	{
		m_mountainsX->alterElevation(0.5);
		m_mountainsZ->alterElevation(0.5);
	}

	if (Input::getKeyPressedOnce(GLFW_KEY_E))
	{
		m_mountainsX->alterElevation(-0.5);
		m_mountainsZ->alterElevation(-0.5);
	}

	/*
		Toggle lights
	*/

	if (Input::getKeyPressedOnce(GLFW_KEY_6))
	{
		if (m_sceneLightManager->getDirectionalLight(0) != nullptr)
			m_sceneLightManager->getDirectionalLight(0)->toggleActive();
	}

	if (Input::getKeyPressedOnce(GLFW_KEY_7))
	{
		if (m_sceneLightManager->getSpotLight(0) != nullptr)
			m_sceneLightManager->getSpotLight(0)->toggleActive();
	}

	if (Input::getKeyPressedOnce(GLFW_KEY_8))
	{
		if (m_sceneLightManager->getPointLight(0) != nullptr)
				m_sceneLightManager->getPointLight(0)->toggleActive();
	}

	if (Input::getKeyPressedOnce(GLFW_KEY_9))
	{
		if (m_sceneLightManager->getPointLight(1) != nullptr)
			m_sceneLightManager->getPointLight(1)->toggleActive();
	}

	if (Input::getKeyPressedOnce(GLFW_KEY_0))
	{
		if (m_sceneLightManager->getPointLight(2) != nullptr)
			m_sceneLightManager->getPointLight(2)->toggleActive();
	}
}

//void Scene::setupShadowStuff()
//{
//
//	glGenFramebuffers(1, &depthMapFBO);
//	glGenTextures(1, &depthMap);
//	glBindTexture(GL_TEXTURE_2D, depthMap);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1280, 720, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
//	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
//	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
//
//	//Attach depth texture to framebuffers depth buffer
//	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
//
//	//Tell framebuffer that no colour data will be rendered 
//	glDrawBuffer(GL_NONE);
//	glReadBuffer(GL_NONE);
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//	float near_plane = 1.0f, far_plane = 7.5f;
//	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
//	lightView = glm::lookAt(glm::vec3(-2.0f, 3.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//	lightSpaceMatrix = lightProjection * lightView;
//
//	EngineStatics::setLightSpaceMatrix(&lightSpaceMatrix);
//	EngineStatics::setDepthMap(&depthMap);
//
//}

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
