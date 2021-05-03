#pragma once
#include <glm\glm.hpp>
#include <vector>

#include <iostream>

//Base class for all 3 lights - provides utility functions they all can use
struct Light
{
	Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
	{
		Ambient = ambient;
		Diffuse = diffuse;
		Specular = specular;
	}
	void toggleActive()
	{
		lightActive = lightActive == true ? false : true;
	}
	glm::vec3	Ambient;
	glm::vec3	Diffuse;
	glm::vec3	Specular;
	bool		lightActive = false;
};

struct PointLight
	:public Light
{
	PointLight(float x = 0.0f, float y = 0.0f, float z = 0.0f, glm::vec3 ambient = glm::vec3(1.0f,1.0f,1.0f), glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f))
		:Light(ambient, diffuse, specular)
	{
		Position.x = x;
		Position.y = y;
		Position.z = z;

		lightActive = true;
	}
	glm::vec3	Position;

	float		Constant = 1.0f;
	float		Linear = 0.07;
	float		Quadratic = 0.017;
	
	//glm::vec3	Ambient { 0.2f, 0.2f, 0.2f };
	//glm::vec3	Diffuse { 1.0f, 1.0f, 1.0f };
	//glm::vec3	Specular{ 0.4f, 0.4f, 0.4f };

	
};

struct DirectionalLight
	:public Light
{
	DirectionalLight(float x = 0.0f, float y = 0.0f, float z = 0.0f, glm::vec3 ambient = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f))
		:Light(ambient, diffuse, specular)
	{
		Direction.x = x;
		Direction.y = y;
		Direction.z = z;

		lightActive = true;
	}
	glm::vec3	Direction;

	//glm::vec3	Ambient { 0.2f, 0.2f, 0.2f };
	//glm::vec3	Diffuse { 0.4f, 0.4f, 0.4f };
	//glm::vec3	Specular{ 0.3f, 0.3f, 0.3f };

};

struct SpotLight
	:public Light
{
	SpotLight(float x = 0.0f, float y = 0.0f, float z = 0.0f, glm::vec3 ambient = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f))
		:Light(ambient, diffuse, specular)
	{
		Position.x = x;
		Position.y = y;
		Position.z = z;
		Direction.x = 0.0f;
		Direction.y = 0.0f;
		Direction.z = 0.0f;

		lightActive = true;
	}
	glm::vec3	Position;
	glm::vec3	Direction;
	float		cutOff = 4.5f;
	float		outerCutOff = 25.5f;

	float		Constant = 1.0f;
	float		Linear = 0.09f;
	float		Quadratic = 0.032f;

	//glm::vec3	Ambient{ 0.0f, 0.0f, 0.0f };
	//glm::vec3	Diffuse{ 0.8f, 0.8f, 0.8f };
	//glm::vec3	Specular{ 1.0f, 1.0f, 1.0f };

};

class LightManager
{
public:
	LightManager();
	~LightManager();

	//Directional Lights
	void setDirectionalLight(float x, float y, float z, int index = 0);
	void addDirectionalLight(float x, float y, float z, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	DirectionalLight* getDirectionalLight(int index = 0) const;

	int getCurrentDirectionalLights() const;

	//Point Lights
	void setPointLight(float x, float y, float z, int index = 0);
	void addPointLight(float x, float y, float z, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	PointLight* getPointLight(int index = 0) const;

	int getCurrentPointLights() const;

	//Spot Lights
	void setSpotLight(float x, float y, float z, int index = 0);
	void addSpotLight(float x, float y, float z, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	SpotLight* getSpotLight(int index = 0) const;

	int getCurrentSpotLights() const;

private:

	//Directional Lights
	std::vector<DirectionalLight*>	m_sceneDirectionalLights;
	const int						m_maxDirectionalLights;
	int								m_currentDirectionalLights;


	//Point Lights
	std::vector<PointLight*>		m_scenePointLights;
	const int						m_maxPointLights;
	int								m_currentPointLights;


	//Spot Lights
	std::vector<SpotLight*>			m_sceneSpotLights;
	const int						m_maxSpotLights;
	int								m_currentSpotLights;
	

};

