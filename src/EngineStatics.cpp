#include "EngineStatics.h"

GLFWwindow* EngineStatics::appWindow = nullptr;

int EngineStatics::screenWidth = 0;

int EngineStatics::screenHeight = 0;

Camera* EngineStatics::camera = nullptr;

glm::mat4* EngineStatics::projectionMatrix;

LightManager* EngineStatics::lightManager = nullptr;

