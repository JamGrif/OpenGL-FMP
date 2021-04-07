#pragma once

#include <vector>

#include "EngineStatics.h"
#include "LightManager.h"
#include "Camera.h"
#include "Model.h"

class Scene
{
public:
	Scene();
	~Scene();

	void initScene();
	void updateScene();

	void addSceneCamera();

private:

	std::vector<Model*> m_sceneMeshes;

	Camera* m_sceneCamera;

	LightManager* m_sceneLightManager;

	Model* cube;


};

