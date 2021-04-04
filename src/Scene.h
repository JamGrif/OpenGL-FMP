#pragma once

#include <vector>

#include "EngineStatics.h"
#include "LightManager.h"
#include "Camera.h"
#include "Mesh.h"

class Scene
{
public:
	Scene();
	~Scene();

	void initScene();
	void updateScene();

	void addSceneCamera();

private:

	std::vector<Mesh*> m_sceneMeshes;

	Camera* m_sceneCamera;

	LightManager* m_sceneLightManager;

	Mesh* cube;


};

