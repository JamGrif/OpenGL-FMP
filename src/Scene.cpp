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
	
	addSceneCamera(0.0f, 2.0f, 0.0f);
	addSceneLightManager();

	//Skybox
	m_sceneMeshes.push_back(new ModelSky());

	//m_sceneFilterFramebuffer = new Framebuffer(false);
	//m_sceneMSAAFrameBuffer = new Framebuffer(true);
	//
	//ModelEnvironment* reflectionModel = new ModelEnvironment(glm::vec3(0.0f, 8.0f, 0.0f));
	//reflectionModel->toggleReflection(true);
	//reflectionModel->setMesh("res/meshes/heart.obj");
	//m_sceneMeshes.push_back(reflectionModel);
	//
	//ModelEnvironment* refractionModel = new ModelEnvironment(glm::vec3(3.0f, 8.0f, -3.0f));
	//refractionModel->toggleRefraction(true);
	//refractionModel->setMesh("res/meshes/heart.obj");
	//m_sceneMeshes.push_back(refractionModel);


	
	m_sceneLightManager->addDirectionalLight(3.0f, -10.0f, 10.0f);
	
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
		ModelLighting* Floor = new ModelLighting(FloorPosRot.at(i), FloorPosRot.at(i + 1));
		Floor->setMesh("res/meshes/plane.obj");
		Floor->setDiffuseTexture("res/textures/concrete2_diff.png");
		Floor->setSpecularTexture("res/textures/concrete2_spec.png");
		Floor->setNormalTexture("res/textures/concrete_norm.png", true);
		//Floor->setHeightTexture("res/textures/concreteBrick_height.png", 0.01f);
		//Floor->setEmissionTexture("res/textures/matrix_emis.png");
		m_sceneMeshes.push_back(Floor);
	}

	
	
	//Z wall
	//std::vector<glm::vec3> WallPosRot =
	//{
	//	glm::vec3(-6.0f, 3.0f, -9.0f), glm::vec3(90.0f, 0.0f, 0.0f),
	//	glm::vec3(0.0f, 3.0f, -9.0f), glm::vec3(90.0f, 0.0f, 0.0f),
	//	glm::vec3(6.0f, 3.0f, -9.0f), glm::vec3(90.0f, 0.0f, 0.0f),
	//	glm::vec3(-6.0f, 3.0f, 9.0f), glm::vec3(-90.0f, 180.0f, 0.0f),
	//	glm::vec3(0.0f, 3.0f, 9.0f), glm::vec3(-90.0f, 180.0f, 0.0f),
	//	glm::vec3(6.0f, 3.0f, 9.0f), glm::vec3(-90.0f, 180.0f, 0.0f)
	//
	//};
	//
	//for (int i = 0; i < WallPosRot.size(); i += 2)
	//{
	//	ModelLighting* wall = new ModelLighting(WallPosRot.at(i), WallPosRot.at(i+1));
	//	wall->setMesh("res/meshes/plane.obj");
	//	wall->setDiffuseTexture("res/textures/cartoonBrick_diff.png");
	//	wall->setSpecularTexture("res/textures/cartoonBrick_spec.png");
	//	wall->setNormalTexture("res/textures/cartoonBrick_norm.png", false);
	//	wall->setHeightTexture("res/textures/cartoonBrick_height.png", 0.05f);
	//	//wall->setEmissionTexture("res/textures/cartoonBrick_emis.png");
	//	m_sceneMeshes.push_back(wall);
	//}
	//
	////Side wall
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
	
	//Crates
	std::vector<glm::vec3> CratePosRot =
	{
		glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		//glm::vec3(-3.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f),
	};
	
	for (int i = 0; i < CratePosRot.size(); i += 2)
	{
		ModelLighting* Crate = new ModelLighting(CratePosRot.at(i), CratePosRot.at(i + 1));
		Crate->setMesh("res/meshes/crate.obj");
		Crate->setDiffuseTexture("res/textures/crate_diff.png");
		Crate->setSpecularTexture("res/textures/crate_spec.png");
		Crate->setNormalTexture("res/textures/crate_norm.png", false);
		m_sceneMeshes.push_back(Crate);
	}

	//std::vector<glm::vec3> CottagePosRot =
	//{
	//	glm::vec3(20.0f, 0.0f, 3.0f), glm::vec3(0.0f, 90.0f, 0.0f)
	//};
	//
	//for (int i = 0; i < CottagePosRot.size(); i += 2)
	//{
	//	ModelLighting* cottage = new ModelLighting(CottagePosRot.at(i), CottagePosRot.at(i + 1));
	//	cottage->setMesh("res/meshes/shack.obj");
	//	cottage->setDiffuseTexture("res/textures/shack_diff.png");
	//	cottage->setSpecularTexture("res/textures/shack_spec.png");
	//	cottage->setNormalTexture("res/textures/shack_norm.png", false);
	//	m_sceneMeshes.push_back(cottage);
	//}
	//
	//std::vector<glm::vec3> CottagePosRot2 
	//{
	//	glm::vec3(20.0f, 0.0f, 7.0f), glm::vec3(0.0f, 90.0f, 0.0f)
	//};
	//
	//for (int i = 0; i < CottagePosRot2.size(); i += 2)
	//{
	//	ModelLighting* cottage = new ModelLighting(CottagePosRot2.at(i), CottagePosRot2.at(i + 1));
	//	cottage->setMesh("res/meshes/shack.obj");
	//	cottage->setDiffuseTexture("res/textures/shack_diff.png");
	//	cottage->setSpecularTexture("res/textures/shack_spec.png");
	//	//cottage->setNormalTexture("res/textures/cottage_norm.png", false);
	//	m_sceneMeshes.push_back(cottage);
	//}


	//Light
	//std::vector<glm::vec3> LightPos =
	//{
	//	glm::vec3(0.0f, 3.0f, 0.0f),
	//	//glm::vec3(15.0f, 3.0f, 3.0f),
	//	//glm::vec3(-3.0f, 3.0f, 5.0f),
	//	//glm::vec3(3.0f, 3.0f, 5.0f)
	//};
	//
	//for (int i = 0; i < LightPos.size(); i++)
	//{
	//	m_sceneLightManager->addPointLight(LightPos.at(i).x, LightPos.at(i).y, LightPos.at(i).z);
	//
	//	ModelBasic* light = new ModelBasic(LightPos.at(i));
	//	light->setMesh("res/meshes/cube.obj");
	//	light->copyPointLight(i);
	//	m_sceneMeshes.push_back(light);
	//}


}

/// <summary>
/// Updates scene objects and models
/// </summary>
void Scene::updateScene()
{
	m_sceneCamera->Update(0.025f);
	//checkForFilterUpdate();

	//m_sceneMSAAFrameBuffer->bindFramebuffer();

	//Draw first pass of all models
	for (Model* m : m_sceneMeshes)
	{
		m->setMatrixValues();
		m->drawPassOne();
	}

	//Draw second pass of all models
	for (Model* m : m_sceneMeshes)
	{
		m->drawPassTwo();
	}

	//Reads from the MSAA buffer and writes it to the Filter buffer
	//glBindFramebuffer(GL_READ_FRAMEBUFFER, m_sceneMSAAFrameBuffer->getFBO());
	//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_sceneFilterFramebuffer->getFBO());
	//glBlitFramebuffer(0, 0, 1280, 720, 0, 0, 1280, 720, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	//
	//m_sceneMSAAFrameBuffer->unbindFramebuffer();
	//
	//m_sceneFilterFramebuffer->draw();

}

/// <summary>
/// Used to change the filter applied to the screen buffer depending on user input. Checked every app loop
/// </summary>
void Scene::checkForFilterUpdate()
{
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
