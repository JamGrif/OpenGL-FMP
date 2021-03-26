#include "LightManager.h"

LightManager::LightManager()
{
	std::cout << "LightManager Initialized" << std::endl;
}

LightManager::~LightManager()
{
	std::cout << "LightManager Destroyed" << std::endl;

	for (PointLight* pl : m_scenePointLights)
	{
		delete pl;
	}
}

void LightManager::addPointLight(float x, float y, float z)
{
	PointLight* point = new PointLight(x, y, z);
	m_scenePointLights.push_back(point);
}

PointLight* LightManager::getPointLight()
{
	return m_scenePointLights.at(0);
}

void LightManager::setPointLight(float x, float z)
{
	m_scenePointLights.at(0)->Position.x += x;
	m_scenePointLights.at(0)->Position.z += z;
}



float* LightManager::getGlobalAmbient()
{
	return globalAmbient;
}
