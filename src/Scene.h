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

	

private:

	void addSceneCamera(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	void addSceneLightManager();

	std::vector<Model*> m_sceneMeshes;

	Camera* m_sceneCamera;

	LightManager* m_sceneLightManager;

	std::vector<glm::vec3> CottagePosRot =
	{
		glm::vec3(0.0f, 0.0f, -10.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 180.0f, 0.0f)
	};

	std::vector<glm::vec3> ShackPosRot =
	{
		glm::vec3(4.5f, 0.5f, -10.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-4.5f, 0.5f, 10.0f), glm::vec3(0.0f, 180.0f, 0.0f)
	};


};

