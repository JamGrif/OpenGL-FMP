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
	glm::vec3 Position;
	
	glm::vec3 Ambient { 0.2f, 0.2f, 0.2f };
	glm::vec3 Diffuse { 1.0f, 1.0f, 1.0f };
	glm::vec3 Specular{ 1.0f, 1.0f, 1.0f };

	float Constant = 1.0f;
	float Linear = 0.09f;
	float Quadratic = 0.032f;

};

struct DirectionalLight
{
	DirectionalLight(float x = 0.0f, float y = 0.0f, float z = 0.0f)
	{
		Direction.x = x;
		Direction.y = y;
		Direction.z = z;
	}

	glm::vec3 Direction;

	glm::vec3 Ambient { 0.2f, 0.2f, 0.2f };
	glm::vec3 Diffuse { 0.4f, 0.4f, 0.4f };
	glm::vec3 Specular{ 1.0f, 1.0f, 1.0f };

};

class LightManager
{
public:
	LightManager();
	~LightManager();

	//Directional Lights
	void setDirectionalLight(float x, float y, float z, int index = 0);
	void addDirectionalLight(float x, float y, float z);
	DirectionalLight* getDirectionalLight(int index = 0);

	//Point Lights
	void setPointLight(float x, float y, float z, int index = 0);
	void addPointLight(float x, float y, float z);
	PointLight* getPointLight(int index = 0);

	//Spot Lights



private:

	//Directional Lights
	std::vector<DirectionalLight*> m_sceneDirectionalLights;
	const int m_maxDirectionalLights;
	int m_currentDirectionalLights;


	//Point Lights
	std::vector<PointLight*> m_scenePointLights;
	const int m_maxPointLights;
	int m_currentPointLights;


	//Spot Lights

	

};

