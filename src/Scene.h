#pragma once

#include <vector>

#include "LightManager.h"
#include "Camera.h"
#include "ModelBasic.h"
#include "ModelLighting.h"
#include "ModelSky.h"
#include "ModelEnvironment.h"
#include "ModelSprite.h"
#include "Framebuffer.h"
#include "Terrain.h"

class Scene
{
public:
	Scene();
	~Scene();

	void initScene();
	void updateScene();

private:

	void addSceneCamera(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	void addSceneLightManager();
	void checkForFilterUpdate();

	void setupShadowStuff();

	std::vector<Model*>		m_sceneMeshes;

	Camera* m_sceneCamera;

	LightManager* m_sceneLightManager;

	Framebuffer* m_sceneMSAAFrameBuffer;		//Scene is drawn to this buffer with MSAA applied
	Framebuffer* m_sceneFilterFramebuffer;	//Recieves info from the MSAAframebuffer which then draws onto a quad which gets displayed to the screen


	//unsigned int depthMapFBO;
	//unsigned int depthMap;
	//glm::mat4 lightProjection = glm::mat4(1.0f);
	//glm::mat4 lightView = glm::mat4(1.0f);
	//glm::mat4 lightSpaceMatrix = glm::mat4(1.0f);

};

