#pragma once

#include <vector>

#include "LightManager.h"
#include "Camera.h"
#include "ModelBasic.h"
#include "ModelLighting.h"
#include "ModelSky.h"
#include "ModelEnvironment.h"
#include "Framebuffer.h"

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

	std::vector<Model*>		m_sceneMeshes;

	Camera*					m_sceneCamera;

	LightManager*			m_sceneLightManager;

	Framebuffer*			m_sceneMSAAFrameBuffer;
	Framebuffer*			m_sceneFilterFramebuffer;

};

