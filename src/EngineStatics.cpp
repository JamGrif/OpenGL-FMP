#include "EngineStatics.h"

GLFWwindow* EngineStatics::appWindow = nullptr;

int EngineStatics::screenWidth = 0;

int EngineStatics::screenHeight = 0;

Camera* EngineStatics::camera = nullptr;

glm::mat4* EngineStatics::projectionMatrix;

glm::mat4* EngineStatics::lightSpaceMatrix;

LightManager* EngineStatics::lightManager = nullptr;

unsigned int* EngineStatics::depthMap = 0;

