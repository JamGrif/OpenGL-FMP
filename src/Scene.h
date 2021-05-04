#pragma once

#include <vector>

#include "LightManager.h"
#include "Camera.h"
#include "ModelBasic.h"
#include "ModelLighting.h"
#include "ModelSky.h"
#include "ModelEnvironment.h"
#include "ModelSprite.h"
#include "Framebuffer.h"
#include "Terrain.h"
#include "ModelGeometry.h"

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

	void updateOnInput();

	void updateSceneLight();

	

	std::vector<Model*>		m_sceneMeshes;

	Camera*					m_sceneCamera;

	LightManager*			m_sceneLightManager;

	Framebuffer*			m_sceneMSAAFrameBuffer;		//Scene is drawn to this buffer with MSAA applied
	Framebuffer*			m_sceneFilterFramebuffer;	//Recieves info from the MSAAframebuffer which then draws onto a quad which gets displayed to the screen

	Terrain*				m_mountainsX;
	Terrain*				m_mountainsZ;


	//void setupShadowStuff();
	 
	//unsigned int depthMapFBO;
	//unsigned int depthMap;
	//glm::mat4 lightProjection = glm::mat4(1.0f);
	//glm::mat4 lightView = glm::mat4(1.0f);
	//glm::mat4 lightSpaceMatrix = glm::mat4(1.0f);

	//Used to move objects around the scene

	//Light showing materials
	float m_materialLightminZ = -5;
	float m_materialLightmaxZ = 9;
	float m_materialLightminX = -25;
	float m_materialLightmaxX = -13;
	bool m_materialLightincZ = true;
	bool m_materialLightincX = true;

	//Light showing normal maps
	float m_normalLightmaxZ = 8;
	float m_normalLightminZ = 23;
	bool m_normalLightincZ = true;

	//Light showing coloured lighting
	float m_r = 0.0f;
	float m_g = 0.0f;
	float m_b = 0.0f;

};

