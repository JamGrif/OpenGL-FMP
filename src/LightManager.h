#pragma once
#include <glm\glm.hpp>
#include <vector>

#include <iostream>

struct PointLight
{
	PointLight(float x = 0.0f, float y = 0.0f, float z = 0.0f)
	{
		Position.x = x;
		Position.y = y;
		Position.z = z;
	}
	
	glm::vec3 Ambient{ 0.0f,0.0f,0.0f};
	glm::vec3 Diffuse{ 1.0f,1.0f,1.0f};
	glm::vec3 Specular{ 1.0f,1.0f,1.0f};

	glm::vec3 Position;
};

class LightManager
{
public:
	LightManager();
	~LightManager();

	void addPointLight(float x, float y, float z);
	PointLight* getPointLight();

	float* getGlobalAmbient();

	void setPointLight(float x, float z);


private:

	float globalAmbient[4] = { 0.7f, 0.7f, 0.7f, 1.0f };

	std::vector<PointLight*> m_scenePointLights;

};

