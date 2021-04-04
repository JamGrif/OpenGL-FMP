#include "LightManager.h"

LightManager::LightManager()
	:m_maxDirectionalLights(1), m_currentDirectionalLights(0),
	m_maxPointLights(4), m_currentPointLights(0),
	m_maxSpotLights(1), m_currentSpotLights(0)
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

void LightManager::setDirectionalLight(float x, float y, float z, int index)
{
	m_sceneDirectionalLights.at(index)->Direction.x = x;
	m_sceneDirectionalLights.at(index)->Direction.y = y;
	m_sceneDirectionalLights.at(index)->Direction.z = z;
}

void LightManager::addDirectionalLight(float x, float y, float z)
{
	//Ensure new directional lights don't exceed the maximum amount allowed
	if (m_currentDirectionalLights < m_maxDirectionalLights)
	{
		DirectionalLight* direction = new DirectionalLight(x, y, z);
		m_sceneDirectionalLights.push_back(direction);
		m_currentDirectionalLights++;

		std::cout << "LIGHTMANAGER->New directional light created at " << x << " " << y << " " << z << std::endl;
	}
}

DirectionalLight* LightManager::getDirectionalLight(int index)
{
	//No directional lights exists
	if (m_currentDirectionalLights == 0)
	{
		return nullptr;
	}

	//Ensure index number is valid
	if (index <= m_sceneDirectionalLights.size())
	{
		return m_sceneDirectionalLights.at(index);
	}
	 
	return nullptr;
	
}

int LightManager::getCurrentDirectionalLights()
{
	return m_currentDirectionalLights;
}

//Changes the position of a specified point light in vector
void LightManager::setPointLight(float x, float y, float z, int index)
{
	m_scenePointLights.at(index)->Position.x = x;
	m_scenePointLights.at(index)->Position.y = y;
	m_scenePointLights.at(index)->Position.z = z;
}

//Add a new point light at specificed position
void LightManager::addPointLight(float x, float y, float z)
{
	//Ensure new point lights don't exceed the maximum amount allowed
	if (m_currentPointLights < m_maxPointLights)
	{
		PointLight* point = new PointLight(x, y, z);
		m_scenePointLights.push_back(point);
		m_currentPointLights++;
		
		std::cout << "LIGHTMANAGER->New point light created at " << x << " " << y << " " << z << std::endl;
	}
}

//Returns the pointlight at specified index in vector
PointLight* LightManager::getPointLight(int index)
{
	//No point lights exists
	if (m_currentPointLights == 0)
	{
		return nullptr;
	}

	//Ensure index number is valid
	if (index < m_scenePointLights.size())
	{
		return m_scenePointLights.at(index);
	}

	return nullptr;
	
}

int LightManager::getCurrentPointLights()
{
	return m_currentPointLights;
}

void LightManager::setSpotLight(float x, float y, float z, int index)
{
	m_sceneSpotLights.at(index)->Position.x = x;
	m_sceneSpotLights.at(index)->Position.y = y;
	m_sceneSpotLights.at(index)->Position.z = z;
}

void LightManager::addSpotLight(float x, float y, float z)
{
	//Ensure new point lights don't exceed the maximum amount allowed
	if (m_currentSpotLights < m_maxSpotLights)
	{
		SpotLight* spot = new SpotLight(x, y, z);
		m_sceneSpotLights.push_back(spot);
		m_currentSpotLights++;

		std::cout << "LIGHTMANAGER->New spot light created at " << x << " " << y << " " << z << std::endl;
	}
}

SpotLight* LightManager::getSpotLight(int index)
{
	//No point lights exists
	if (m_currentSpotLights == 0)
	{
		return nullptr;
	}

	//Ensure index number is valid
	if (index <= m_sceneSpotLights.size())
	{
		return m_sceneSpotLights.at(index);
	}

	return nullptr;
}

int LightManager::getCurrentSpotLights()
{
	return m_currentSpotLights;
}





