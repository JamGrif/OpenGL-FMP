#pragma once
#include <glm\glm.hpp>
#include "Camera.h"
#include "LightManager.h"

class EngineStatics
{
public:

	static GLFWwindow* getAppWindow() { return appWindow; }
	static void setAppWindow(GLFWwindow* w) { appWindow = w; }

	static Camera* getCamera() { return camera; }
	static void setCamera(Camera* c) { camera = c; }
	
	static glm::mat4* getProjectionMatrix() { return projectionMatrix; }
	static void setProjectionMatrix(glm::mat4* pm) { projectionMatrix = pm; }

	static LightManager* getLightManager() { return lightManager; }
	static void setLightManager(LightManager* lm) { lightManager = lm; }



private:

	static GLFWwindow* appWindow;

	static Camera* camera;

	static glm::mat4* projectionMatrix;

	static LightManager* lightManager;

};

