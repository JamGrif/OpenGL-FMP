#include "Scene.h"

#include "EngineStatics.h"

#include "Input.h"

Scene::Scene()
	:m_sceneCamera(nullptr), m_sceneLightManager(nullptr), m_sceneMSAAFrameBuffer(nullptr), m_sceneFilterFramebuffer(nullptr), m_mountainsX(nullptr), m_mountainsZ(nullptr),
	m_materialLightMinZ(-5.0f), m_materialLightMaxZ(9.0f), m_materialLightMinX(-25.0f), m_materialLightMaxX(-13.0f), m_materialLightIncZ(true),
	m_materialLightIncX(true), m_normalLightMaxZ(8.0f), m_normalLightMinZ(23.0f), m_normalLightIncZ(true), m_lightR(0.0f), m_lightG(0.0f), m_lightB(0.0f)
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
	//UNUSED function used for shadow buffers setup
	//setupShadowStuff();
	
	addSceneCamera(0.0f, 2.0f, 0.0f);
	addSceneLightManager();

	//Skybox
	m_sceneMeshes.push_back(new ModelSky());

	//Framebuffers
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
	
	//Reflection environment map object
	ModelEnvironment* reflectionModel = new ModelEnvironment(glm::vec3(14.0f, 0.5f, -7.0f));
	reflectionModel->toggleReflection(true);
	reflectionModel->setMesh("res/meshes/heart.obj");
	reflectionModel->SetXScale(1);
	reflectionModel->SetYScale(1);
	reflectionModel->SetZScale(1);
	m_sceneMeshes.push_back(reflectionModel);
	
	//Refraction environment map object
	ModelEnvironment* refractionModel = new ModelEnvironment(glm::vec3(22.0f, 0.5f, -7.0f));
	refractionModel->toggleRefraction(true);
	refractionModel->setMesh("res/meshes/heart.obj");
	refractionModel->SetXScale(1);
	refractionModel->SetYScale(1);
	refractionModel->SetZScale(1);
	m_sceneMeshes.push_back(refractionModel);

	//Grass
	std::vector<glm::vec3> grassPos =
	{
		//Position
		glm::vec3(4.0f, -0.5f, 13.0f),
		glm::vec3(8.0f, -0.5f, 18.0f),
		glm::vec3(14.0f, -0.5f, 15.0f),
		glm::vec3(19.0f, -0.5f, -5.0f),
		glm::vec3(23.0f, -0.5f, 10.0f),
		glm::vec3(-4.0f, -0.5f,  13.0f),
		glm::vec3(-8.0f, -0.5f,  18.0f),
		glm::vec3(-14.0f, -0.5f, 15.0f),
		glm::vec3(-19.0f, -0.5f, 17.0f)
	};
	
	for (int i = 0; i < grassPos.size(); i++)
	{
		ModelSprite* grass = new ModelSprite(grassPos.at(i));
		grass->setSprite("res/textures/grass_sprite.png");
		grass->SetXScale(0.4f);
		grass->SetYScale(0.4f);
		grass->SetZScale(0.4f);
		m_sceneMeshes.push_back(grass);
	}

	//Lighting
	m_sceneLightManager->addDirectionalLight(-2.0f, -3.0f, -1.0f, glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.3f, 0.3f, 0.3f));
	
	m_sceneLightManager->addSpotLight(0.0f, 0.0f, 0.0f, glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.8f,0.8f,0.8f), glm::vec3(1.0f, 1.0f, 1.0f));
	m_sceneLightManager->getSpotLight(0)->toggleActive(); //Turns off spotlight by default

	//Point lights
	std::vector<glm::vec3> LightPosAmbDifSpc =
	{
		//Position						//Ambient						//Diffuse						//Specular
		glm::vec3(0.0f, 3.0f, 0.0f),	glm::vec3(0.2f, 0.2f, 0.2f),	glm::vec3(1.0f, 1.0f, 1.0f),		glm::vec3(0.4f, 0.4f, 0.4f),		//House light
		glm::vec3(-18.0f, 2.0f, 0.0f),	glm::vec3(0.2f, 0.2f, 0.2f),	glm::vec3(1.0f, 1.0f, 1.0f),	glm::vec3(0.4f, 0.4f, 0.4f),	//Material showcase light
		glm::vec3(18.0f, 2.0f, 14.0f),	glm::vec3(0.2f, 0.2f, 0.2f),	glm::vec3(1.0f, 1.0f, 1.0f),	glm::vec3(0.4f, 0.4f, 0.4f),	//Normal showcase light
		glm::vec3(-20.0f, 2.0f, 20.0f), glm::vec3(0.2f, 0.2f, 0.2f),	glm::vec3(1.0f, 1.0f, 1.0f),	glm::vec3(0.4f, 0.4f, 0.4f),	//Coloured lighting
	};

	for (int i = 0; i < LightPosAmbDifSpc.size(); i += 4)
	{
		m_sceneLightManager->addPointLight(LightPosAmbDifSpc.at(i).x, LightPosAmbDifSpc.at(i).y, LightPosAmbDifSpc.at(i).z, LightPosAmbDifSpc.at(i + 1), LightPosAmbDifSpc.at(i + 2), LightPosAmbDifSpc.at(i + 3));
	}

	//Objects that copy lights
	for (int i = 0; i < m_sceneLightManager->getCurrentPointLights(); i++)
	{
		ModelBasic* light = new ModelBasic(LightPosAmbDifSpc.at(i));
		light->setMesh("res/meshes/cube.obj");
		light->copyPointLight(i);
		m_sceneMeshes.push_back(light);
	}

	//Floor
	std::vector<glm::vec3> FloorPosRot =
	{
		//Position	
		glm::vec3(0.0f, 0.0f, 0.0f), 
		glm::vec3(6.0f, 0.0f, 0.0f), 
		glm::vec3(-6.0f, 0.0f, 0.0f), 
		glm::vec3(0.0f, 0.0f, 6.0f), 
		glm::vec3(6.0f, 0.0f, 6.0f), 
		glm::vec3(-6.0f, 0.0f, 6.0f),
		glm::vec3(0.0f, 0.0f, -6.0f), 
		glm::vec3(6.0f, 0.0f, -6.0f), 
		glm::vec3(-6.0f, 0.0f, -6.0f)
	};
	
	for (int i = 0; i < FloorPosRot.size(); i ++)
	{
		ModelLighting* Floor = new ModelLighting(FloorPosRot.at(i));
		Floor->setMesh("res/meshes/plane.obj");
		Floor->setDiffuseTexture("res/textures/carpet_diff.png");
		Floor->setSpecularTexture("res/textures/carpet_spec.png");
		Floor->setNormalTexture("res/textures/carpet_norm.png", false);
		Floor->setHeightTexture("res/textures/carpet_height.png", 0.05f);
		m_sceneMeshes.push_back(Floor);
	}
	
	std::vector<glm::vec3> ConcreteFloor =
	{
		//Position						//Rotation
		glm::vec3(0.0f, -0.8f, 0.0f),	glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(6.0f, -0.8f, 0.0f),	glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-6.0f, -0.8f, 0.0f),	glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, -0.8f, 6.0f),	glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(6.0f, -0.8f, 6.0f),	glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-6.0f, -0.8f, 6.0f),	glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, -0.8f, -6.0f),	glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(6.0f, -0.8f, -6.0f),	glm::vec3(0.0f, 0.0f, 0.0f),
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
		//Position						//Rotation
		glm::vec3(-9.0f, 2.8f, -6.0f),	glm::vec3(90.0f, 0.0f, -90.0f),
		glm::vec3(-9.0f, 2.8f, 0.0f),	glm::vec3(90.0f, 0.0f, -90.0f),
		glm::vec3(-9.0f, 2.8f, 6.0f),	glm::vec3(90.0f, 0.0f, -90.0f),
		glm::vec3(9.0f, 2.8f, -6.0f),	glm::vec3(90.0f, 0.0f, 90.0f),
		glm::vec3(9.0f, 2.8f, 0.0f),	glm::vec3(90.0f, 0.0f, 90.0f),
		glm::vec3(9.0f, 2.8f, 6.0f),	glm::vec3(90.0f, 0.0f, 90.0f),
		glm::vec3(-6.0f, 2.8f, -9.0f),	glm::vec3(90.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 2.8f, -9.0f),	glm::vec3(90.0f, 0.0f, 0.0f),
		glm::vec3(6.0f, 2.8f, -9.0f),	glm::vec3(90.0f, 0.0f, 0.0f),
		glm::vec3(-6.0f, 2.8f, 9.0f),	glm::vec3(-90.0f, 180.0f, 0.0f),
		glm::vec3(6.0f, 2.8f, 9.0f),	glm::vec3(-90.0f, 180.0f, 0.0f)
	};
	
	for (int i = 0; i < SideWallPosRot.size(); i += 2)
	{
		ModelLighting* wall = new ModelLighting(SideWallPosRot.at(i), SideWallPosRot.at(i + 1));
		wall->setMesh("res/meshes/wall.obj");
		wall->setDiffuseTexture("res/textures/woodWall_diff.png");
		wall->setSpecularTexture("res/textures/woodWall_spec.png");
		wall->setNormalTexture("res/textures/woodWall_norm.png", false);
		wall->setHeightTexture("res/textures/woodWall_height.png", 0.04f);
		m_sceneMeshes.push_back(wall);
	}
	
	//Roof
	std::vector<glm::vec3> RoofPosRot =
	{
		//Position						//Rotation
		glm::vec3(0.0f, 5.8f, 0.0f),	glm::vec3(0.0f, 0.0f, 180.0f),
		glm::vec3(6.0f, 5.8f, 0.0f),	glm::vec3(0.0f, 0.0f, 180.0f),
		glm::vec3(-6.0f, 5.8f, 0.0f),	glm::vec3(0.0f, 0.0f, 180.0f),
		glm::vec3(0.0f, 5.8f, 6.0f),	glm::vec3(0.0f, 0.0f, 180.0f),
		glm::vec3(6.0f, 5.8f, 6.0f),	glm::vec3(0.0f, 0.0f, 180.0f),
		glm::vec3(-6.0f, 5.8f, 6.0f),	glm::vec3(0.0f, 0.0f, 180.0f),
		glm::vec3(0.0f, 5.8f, -6.0f),	glm::vec3(0.0f, 0.0f, 180.0f),
		glm::vec3(6.0f, 5.8f, -6.0f),	glm::vec3(0.0f, 0.0f, 180.0f),
		glm::vec3(-6.0f, 5.8f, -6.0f),	glm::vec3(0.0f, 0.0f, 180.0f)
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
	ModelLighting* dresser = new ModelLighting(glm::vec3(0.0f, 0.0f, -7.75f), glm::vec3(0.0f, 0.0f, 0.0f));
	dresser->setMesh("res/meshes/dresser.obj");
	dresser->setDiffuseTexture("res/textures/dresser_diff.png");
	dresser->setSpecularTexture("res/textures/dresser_spec.png");
	dresser->setNormalTexture("res/textures/dresser_norm.png", false);
	m_sceneMeshes.push_back(dresser);
	
	
	//Front Stair
	ModelLighting* stair = new ModelLighting(glm::vec3(0.0f, -2.25f, 10.0f), glm::vec3(0.0f, 90.0f, 0.0f));
	stair->setMesh("res/meshes/stairs.obj");
	stair->setDiffuseTexture("res/textures/stairs_diff.png");
	stair->setSpecularTexture("res/textures/stairs_spec.png");
	stair->setNormalTexture("res/textures/stairs_norm.png", false);
	m_sceneMeshes.push_back(stair);
	
	
	//Grass
	std::vector<glm::vec3> GrassPosRot =
	{
		//Position							
		glm::vec3(0.0f, -1.5f, 0.0f),		
		glm::vec3(6.0f, -1.5f, 0.0f),		
		glm::vec3(-6.0f,-1.5f,  0.0f),		
		glm::vec3(0.0f, -1.5f, 6.0f),		
		glm::vec3(6.0f, -1.5f, 6.0f),		
		glm::vec3(-6.0f,-1.5f,  6.0f),		
		glm::vec3(0.0f, -1.5f, -6.0f),		
		glm::vec3(6.0f, -1.5f, -6.0f),		
		glm::vec3(-6.0f,-1.5f,  -6.0f),		
	
		glm::vec3(-18.0f, -1.5f, 0.0f),		
		glm::vec3(-12.0f, -1.5f, 0.0f),		
		glm::vec3(-24.0f, -1.5f, 0.0f),		
		glm::vec3(-18.0f, -1.5f, 6.0f),		
		glm::vec3(-12.0f, -1.5f, 6.0f),		
		glm::vec3(-24.0f, -1.5f, 6.0f),		
		glm::vec3(-18.0f, -1.5f, -6.0f),	
		glm::vec3(-12.0f, -1.5f, -6.0f),	
		glm::vec3(-24.0f, -1.5f, -6.0f),	
	
		glm::vec3(18.0f, -1.5f, 0.0f),		
		glm::vec3(12.0f, -1.5f, 0.0f),		
		glm::vec3(24.0f, -1.5f, 0.0f),		
		glm::vec3(18.0f, -1.5f, 6.0f),		
		glm::vec3(12.0f, -1.5f, 6.0f),		
		glm::vec3(24.0f, -1.5f, 6.0f),		
		glm::vec3(18.0f, -1.5f, -6.0f),		
		glm::vec3(12.0f, -1.5f, -6.0f),		
		glm::vec3(24.0f, -1.5f, -6.0f),		
	
		//
		glm::vec3(0.0f, -1.5f, 18.0f),		
		glm::vec3(6.0f, -1.5f, 18.0f),		
		glm::vec3(-6.0f,-1.5f,  18.0f),		
		glm::vec3(0.0f, -1.5f, 24.0f),		
		glm::vec3(6.0f, -1.5f, 24.0f),		
		glm::vec3(-6.0f,-1.5f,  24.0f),		
		glm::vec3(0.0f, -1.5f, 12.0f),		
		glm::vec3(6.0f, -1.5f, 12.0f),		
		glm::vec3(-6.0f,-1.5f,  12.0f),		
	
		glm::vec3(-18.0f, -1.5f, 18.0f),	
		glm::vec3(-12.0f, -1.5f, 18.0f),	
		glm::vec3(-24.0f, -1.5f, 18.0f),	
		glm::vec3(-18.0f, -1.5f, 24.0f),	
		glm::vec3(-12.0f, -1.5f, 24.0f),	
		glm::vec3(-24.0f, -1.5f, 24.0f),	
		glm::vec3(-18.0f, -1.5f, 12.0f),	
		glm::vec3(-12.0f, -1.5f, 12.0f),	
		glm::vec3(-24.0f, -1.5f, 12.0f),	
	
		glm::vec3(18.0f, -1.5f, 18.0f),		
		glm::vec3(12.0f, -1.5f, 18.0f),		
		glm::vec3(24.0f, -1.5f, 18.0f),		
		glm::vec3(18.0f, -1.5f, 24.0f),		
		glm::vec3(12.0f, -1.5f, 24.0f),		
		glm::vec3(24.0f, -1.5f, 24.0f),		
		glm::vec3(18.0f, -1.5f, 12.0f),		
		glm::vec3(12.0f, -1.5f, 12.0f),		
		glm::vec3(24.0f, -1.5f, 12.0f),		
	};
	
	for (int i = 0; i < GrassPosRot.size(); i ++)
	{
		ModelLighting* Grass = new ModelLighting(GrassPosRot.at(i));
		Grass->setMesh("res/meshes/plane.obj");
		Grass->setDiffuseTexture("res/textures/grass_diff.png");
		Grass->setSpecularTexture("res/textures/grass_spec.png");
		m_sceneMeshes.push_back(Grass);
	}
	
	
	//Side wall
	std::vector<glm::vec3> OutsideWall =
	{
		//Position						//Rotation
		glm::vec3(-12.0f, 1.5f, -9.0f), glm::vec3(90.0f, 0.0f, 0.0f),
		glm::vec3(-18.0f, 1.5f, -9.0f), glm::vec3(90.0f, 0.0f, 0.0f),
		glm::vec3(-24.0f, 1.5f, -9.0f), glm::vec3(90.0f, 0.0f, 0.0f),

		glm::vec3(-27.0f, 1.5f, -6.0f), glm::vec3(90.0f, 0.0f, -90.0f),
		glm::vec3(-27.0f, 1.5f, 0.0f),	glm::vec3(90.0f, 0.0f, -90.0f),
		glm::vec3(-27.0f, 1.5f, 6.0f),	glm::vec3(90.0f, 0.0f, -90.0f),
		glm::vec3(-27.0f, 1.5f, 12.0f), glm::vec3(90.0f, 0.0f, -90.0f),
		glm::vec3(-27.0f, 1.5f, 18.0f), glm::vec3(90.0f, 0.0f, -90.0f),
		glm::vec3(-27.0f, 1.5f, 24.0f), glm::vec3(90.0f, 0.0f, -90.0f),

		glm::vec3(-24.0f, 1.5f, 27.0f), glm::vec3(-90.0f, 0.0f, 0.0f),
		glm::vec3(-18.0f, 1.5f, 27.0f), glm::vec3(-90.0f, 0.0f, 0.0f),
		glm::vec3(-12.0f, 1.5f, 27.0f), glm::vec3(-90.0f, 0.0f, 0.0f),
		glm::vec3(-6.0f, 1.5f, 27.0f),	glm::vec3(-90.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.5f, 27.0f),	glm::vec3(-90.0f, 0.0f, 0.0f),
		glm::vec3(6.0f, 1.5f, 27.0f),	glm::vec3(-90.0f, 0.0f, 0.0f),
		glm::vec3(12.0f, 1.5f, 27.0f),	glm::vec3(-90.0f, 0.0f, 0.0f),
		glm::vec3(18.0f, 1.5f, 27.0f),	glm::vec3(-90.0f, 0.0f, 0.0f),
		glm::vec3(24.0f, 1.5f, 27.0f),	glm::vec3(-90.0f, 0.0f, 0.0f),

		glm::vec3(27.0f, 1.5f, 24.0f),	glm::vec3(90.0f, 0.0f, 90.0f),
		glm::vec3(27.0f, 1.5f, 18.0f),	glm::vec3(90.0f, 0.0f, 90.0f),
		glm::vec3(27.0f, 1.5f, 12.0f),	glm::vec3(90.0f, 0.0f, 90.0f),
		glm::vec3(27.0f, 1.5f, 6.0f),	glm::vec3(90.0f, 0.0f, 90.0f),
		glm::vec3(27.0f, 1.5f, 0.0f),	glm::vec3(90.0f, 0.0f, 90.0f),
		glm::vec3(27.0f, 1.5f, -6.0f),	glm::vec3(90.0f, 0.0f, 90.0f),

		glm::vec3(12.0f, 1.5f, -9.0f),	glm::vec3(90.0f, 0.0f, 0.0f),
		glm::vec3(18.0f, 1.5f, -9.0f),	glm::vec3(90.0f, 0.0f, 0.0f),
		glm::vec3(24.0f, 1.5f, -9.0f),	glm::vec3(90.0f, 0.0f, 0.0f),
	};
	
	for (int i = 0; i < OutsideWall.size(); i += 2)
	{
		ModelLighting* wall = new ModelLighting(OutsideWall.at(i), OutsideWall.at(i + 1));
		wall->setMesh("res/meshes/plane.obj");
		wall->setDiffuseTexture("res/textures/concreteBrick_diff.png");
		wall->setSpecularTexture("res/textures/concreteBrick_spec.png");
		wall->setNormalTexture("res/textures/concreteBrick_norm.png", false);
		wall->setHeightTexture("res/textures/concreteBrick_height.png", 0.01f);
		m_sceneMeshes.push_back(wall);
	}

	//Shed
	std::vector<glm::vec3> ShedPos =
	{
		//Position							//Rotation
		glm::vec3(-38.0f, 1.0f, 0.0f),		glm::vec3(0.0f, 210.0f, 0.0f),
		glm::vec3(38.0f, 1.0f, 10.0f),		glm::vec3(0.0f, -210.0f, 0.0f),
		glm::vec3(13.0f, 14.0f, -67.0f),	glm::vec3(0.0f, -210.0f, 0.0f)
	};

	for (int i = 0; i < ShedPos.size(); i += 2)
	{
		ModelLighting* shed = new ModelLighting(ShedPos.at(i), ShedPos.at(i + 1));
		shed->setMesh("res/meshes/shed.obj");
		shed->setDiffuseTexture("res/textures/shed_diff.png");
		shed->setSpecularTexture("res/textures/shed_spec.png");
		shed->setNormalTexture("res/textures/shed_norm.png", false);
		m_sceneMeshes.push_back(shed);
	}

	//Cottage
	std::vector<glm::vec3> CottagePos =
	{
		//Position							//Rotation
		glm::vec3(-23.0f, 0.0f, -20.0f),	glm::vec3(0.0f, 130.0f, 0.0f),
		glm::vec3(20.0f, 1.0f, 37.0f),		glm::vec3(0.0f, 45.0f, 0.0f)
	};

	for (int i = 0; i < CottagePos.size(); i += 2)
	{
		ModelLighting* cottage = new ModelLighting(CottagePos.at(i), CottagePos.at(i + 1));
		cottage->setMesh("res/meshes/cottage.obj");
		cottage->setDiffuseTexture("res/textures/cottage_diff.png");
		cottage->setSpecularTexture("res/textures/cottage_spec.png");
		cottage->setNormalTexture("res/textures/cottage_norm.png", false);
		m_sceneMeshes.push_back(cottage);
	}

	//Sign
	std::vector<glm::vec3> SignPosRot =
	{
		//Position							//Rotation
		glm::vec3(2.0f, -3.0f, 25.75f),		glm::vec3(0.0f, 180.0f, 0.0f),		//Breating barrel 
		glm::vec3(-3.0f, -1.3f, -4.0f),		glm::vec3(0.0f, 30.0f, 0.0f),		//Q and E tessellation shader 
		glm::vec3(7.0f, -1.3f, -4.5f),		glm::vec3(0.0f, -90.0f, 0.0f),		//Fake depth from height / normal 
		glm::vec3(3.0f, -1.3f, -4.0f),		glm::vec3(0.0f, -30.0f, 0.0f),		//Sign text from emission 
		glm::vec3(11.0f, -3.0f, -4.75f),	glm::vec3(0.0f, 30.0f, 0.0f),		//Environment reflect 
		glm::vec3(25.0f, -3.0f, -4.75f),	glm::vec3(0.0f, -30.0f, 0.0f),		//Environment refract 
		glm::vec3(25.0f, -3.0f, 15.0f),		glm::vec3(0.0f, -90.0f, 0.0f),		//Normal vs no normal 
		glm::vec3(-15.0f, -3.0f, -8.25f),	glm::vec3(0.0f, 0.0f, 0.0f),		//Different materials 
		glm::vec3(-26.0f, -3.0f, 17.75f),	glm::vec3(0.0f, 90.0f, 0.0f),		//Colour lighting
		glm::vec3(-23.0f, -3.0f, -8.25f),	glm::vec3(0.0f, 0.0f, 0.0f),		//disable lights to show 
		glm::vec3(0.0f, -1.3f, -4.0f),		glm::vec3(0.0f, 0.0f, 0.0f),		//toggle lights	
		glm::vec3(0.0f, 1.0f, -8.0f),		glm::vec3(0.0f, 0.0f, 0.0f),		//change filters 
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
		"res/textures/signs/sign9_emis.png",
		"res/textures/signs/sign10_emis.png",
		"res/textures/signs/sign11_emis.png",
		"res/textures/signs/sign12_emis.png"
	};

	int signNum = 0;
	for (int i = 0; i < SignPosRot.size(); i += 2)
	{
		ModelLighting* sign = new ModelLighting(SignPosRot.at(i), SignPosRot.at(i + 1));
		sign->setMesh("res/meshes/signpost.obj");
		sign->setDiffuseTexture("res/textures/sign_diff.png");
		sign->setSpecularTexture("res/textures/sign_spec.png");
		sign->setNormalTexture("res/textures/sign_norm.png", false);
		sign->setEmissionTexture(SignTex.at(signNum));
		sign->SetXScale(0.75);
		sign->SetYScale(0.75);
		sign->SetZScale(0.75);
		m_sceneMeshes.push_back(sign);
		signNum++;
	}

	/*
		Floor showcase
	*/

	std::vector<const char*> floorMaterials =
	{
		//Diffuse map							//Specular Map							//Normal Map 
		"res/textures/metal_diff.png",			"res/textures/metal_spec.png",			"res/textures/metal_norm.png",			//not normalized texture
		"res/textures/tile_diff.png",			"res/textures/tile_spec.png",			"res/textures/tile_norm.png",			//not normalized texture
		"res/textures/sand_diff.png",			"res/textures/sand_spec.png",			"res/textures/sand_norm.png",			//not normalized texture
		"res/textures/marble_diff.png",			"res/textures/marble_spec.png",			"res/textures/marble_norm.png",			//not normalized texture
		"res/textures/hieroglyphs_diff.png",	"res/textures/hieroglyphs_spec.png",	"res/textures/hieroglyphs_norm.png",	//not normalized texture	
		"res/textures/metalHammer_diff.png",	"res/textures/metalHammer_spec.png",	"res/textures/metalHammer_norm.png",	//normalized texture		
		"res/textures/skullGround_diff.png",	"res/textures/skullGround_spec.png",	"res/textures/skullGround_norm.png",	//normalized texture
		"res/textures/rock_diff.png",			"res/textures/rock_spec.png",			"res/textures/rock_norm.png",			//normalized texture
		"res/textures/concrete2_diff.png",		"res/textures/concrete2_spec.png",		"res/textures/concrete2_norm.png",		//normalized texture
	};

	std::vector<glm::vec3> floorPos =
	{
		//Position
		glm::vec3(-24.0f, -1.25, -4.0),
		glm::vec3(-18.0f, -1.25, -4.0),
		glm::vec3(-12.0f, -1.25, -4.0),
		glm::vec3(-24.0f, -1.25, 2.0),
		glm::vec3(-18.0f, -1.25, 2.0),
		glm::vec3(-18.0f, -1.25, 8.0),
		glm::vec3(-24.0f, -1.25, 8.0),
		glm::vec3(-12.0f, -1.25, 2.0),
		glm::vec3(-12.0f, -1.25, 8.0),
		
	};

	int materialNum = 0;
	for (int i = 0; i < floorPos.size(); i++)
	{
		ModelLighting* floor = new ModelLighting(floorPos.at(i));
		floor->setMesh("res/meshes/plane.obj");
		floor->setDiffuseTexture(floorMaterials.at(materialNum));
		floor->setSpecularTexture(floorMaterials.at(materialNum+1));
		if (materialNum >= 6) 
		{
			floor->setNormalTexture(floorMaterials.at(materialNum + 2), true);
		}
		else
		{
			floor->setNormalTexture(floorMaterials.at(materialNum + 2), false);
		}
		
		m_sceneMeshes.push_back(floor);
		materialNum += 3;
	}

	//Normal vs no normal
	std::vector<glm::vec3> planePos =
	{
		//Position						//Rotation
		glm::vec3(25.0f, 1.0f, 6.0f),	glm::vec3(90.0f, 0.0f, 90.0f),
		glm::vec3(25.0f, 1.0f, 12.0f),	glm::vec3(90.0f, 0.0f, 90.0f),
		glm::vec3(25.0f, 1.0f, 18.0f),	glm::vec3(90.0f, 0.0f, 90.0f),
		glm::vec3(25.0f, 1.0f, 24.0f),	glm::vec3(90.0f, 0.0f, 90.0f),
		glm::vec3(22.0f, 1.0f, 26.0f),	glm::vec3(90.0f, 0.0f, 180.0f),
		glm::vec3(16.0f, 1.0f, 26.0f),	glm::vec3(90.0f, 0.0f, 180.0f),
	};

	ModelLighting * leather = new ModelLighting(planePos.at(0), planePos.at(1));
	leather->setMesh("res/meshes/plane.obj");
	leather->setDiffuseTexture("res/textures/leather_diff.png");
	m_sceneMeshes.push_back(leather);

	ModelLighting* leather2 = new ModelLighting(planePos.at(2), planePos.at(3));
	leather2->setMesh("res/meshes/plane.obj");
	leather2->setDiffuseTexture("res/textures/leather_diff.png");
	leather2->setNormalTexture("res/textures/leather_norm.png", false);
	m_sceneMeshes.push_back(leather2);

	ModelLighting* concrete = new ModelLighting(planePos.at(4), planePos.at(5));
	concrete->setMesh("res/meshes/plane.obj");
	concrete->setDiffuseTexture("res/textures/concreteExample_diff.png");
	m_sceneMeshes.push_back(concrete);

	ModelLighting* concrete2 = new ModelLighting(planePos.at(6), planePos.at(7));
	concrete2->setMesh("res/meshes/plane.obj");
	concrete2->setDiffuseTexture("res/textures/concreteExample_diff.png");
	concrete2->setNormalTexture("res/textures/concreteExample_norm.png", false);
	m_sceneMeshes.push_back(concrete2);

	ModelLighting* brick = new ModelLighting(planePos.at(8), planePos.at(9));
	brick->setMesh("res/meshes/plane.obj");
	brick->setDiffuseTexture("res/textures/cartoonBricks_diff.png");
	m_sceneMeshes.push_back(brick);

	ModelLighting* brick2 = new ModelLighting(planePos.at(10), planePos.at(11));
	brick2->setMesh("res/meshes/plane.obj");
	brick2->setDiffuseTexture("res/textures/cartoonBricks_diff.png");
	brick2->setNormalTexture("res/textures/cartoonBricks_norm.png", false);
	m_sceneMeshes.push_back(brick2);
	
}

/// <summary>
/// Updates scene objects and models
/// </summary>
void Scene::updateScene()
{
	//Update functions
	updateSceneLight();
	updateOnInput();
	m_sceneCamera->Update(EngineStatics::getDeltaTime());

	//Bind MSAA for object drawing
	m_sceneMSAAFrameBuffer->bindFramebuffer();

	//UNUSED code that was used for shadowing ---
	//Draw first pass of all models
	//glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	//glClear(GL_DEPTH_BUFFER_BIT);
	
	for (Model* m : m_sceneMeshes)
	{
		m->setMatrixValues();
		//m->drawPassOne(); //drawPassOne is unused as it was used for shadowing in the ModelLighting class
	}

	//UNUSED code that was used for shadowing ---
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
	
	//Draw screen filter to buffer
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

	if (Input::getKeyPressedOnce(GLFW_KEY_MINUS))
	{
		if (m_sceneLightManager->getPointLight(3) != nullptr)
			m_sceneLightManager->getPointLight(3)->toggleActive();
	}
}

/// <summary>
/// Alters the lights position or colour every frame 
/// </summary>
void Scene::updateSceneLight()
{
	/*
		Light over material showcase
	*/

	if (m_materialLightIncZ)
	{
		m_sceneLightManager->getPointLight(1)->Position.z += 0.075f;
		if (m_sceneLightManager->getPointLight(1)->Position.z >= m_materialLightMaxZ)
		{
			m_sceneLightManager->getPointLight(1)->Position.z = m_materialLightMaxZ;
			m_materialLightIncZ = false;
		}
	}
	else
	{
		m_sceneLightManager->getPointLight(1)->Position.z -= 0.075f;
		if (m_sceneLightManager->getPointLight(1)->Position.z <= m_materialLightMinZ)
		{
			m_sceneLightManager->getPointLight(1)->Position.z = m_materialLightMinZ;
			m_materialLightIncZ = true;
		}
	}

	if (m_materialLightIncX)
	{
		m_sceneLightManager->getPointLight(1)->Position.x += 0.1f;
		if (m_sceneLightManager->getPointLight(1)->Position.x >= m_materialLightMaxX)
		{
			m_sceneLightManager->getPointLight(1)->Position.x = m_materialLightMaxX;
			m_materialLightIncX = false;
		}
	}
	else
	{
		m_sceneLightManager->getPointLight(1)->Position.x -= 0.1f;
		if (m_sceneLightManager->getPointLight(1)->Position.x <= m_materialLightMinX)
		{
			m_sceneLightManager->getPointLight(1)->Position.x = m_materialLightMinX;
			m_materialLightIncX = true;
		}
	}

	/*
		Light showing normals
	*/

	if (m_normalLightIncZ)
	{
		m_sceneLightManager->getPointLight(2)->Position.z += 0.05f;
		if (m_sceneLightManager->getPointLight(2)->Position.z >= m_normalLightMaxZ)
		{
			m_sceneLightManager->getPointLight(2)->Position.z = m_normalLightMaxZ;
			m_normalLightIncZ = false;
		}
	}
	else
	{
		m_sceneLightManager->getPointLight(2)->Position.z += 0.05f;
		if (m_sceneLightManager->getPointLight(2)->Position.z >= m_normalLightMinZ)
		{
			m_sceneLightManager->getPointLight(2)->Position.z = m_normalLightMinZ;
			m_normalLightIncZ = true;
		}
	}

	/*
		Coloured Lighting
	*/

	m_lightR -= 0.001f;
	if (m_lightR <= 0.0f)
		m_lightR = 1.0f;

	m_lightG += 0.003f;
	if (m_lightG >= 1.0f)
		m_lightG = 0.0f;

	m_lightB += 0.002f;
	if (m_lightB >= 1.0f)
		m_lightB = 0.0f;

	m_sceneLightManager->getPointLight(3)->Ambient = glm::vec3(m_lightR,  m_lightG,  m_lightB);
	m_sceneLightManager->getPointLight(3)->Diffuse = glm::vec3(m_lightR,  m_lightG,  m_lightB);
	m_sceneLightManager->getPointLight(3)->Specular = glm::vec3(m_lightR, m_lightG,  m_lightB);
}

//UNUSED function used for shadowing - this was to setup the depthMap texture which the scene would be drawn too during the first pass of some models
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
