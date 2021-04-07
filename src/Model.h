#pragma once


#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "EngineStatics.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>


class Model
{
public:
	Model();
	~Model();

	void setMesh(const char* meshFilePath);

	void drawModel();

	void setDiffuseTexture(const char* texturePath);
	void setSpecularTexture(const char* texturePath);

	void		SetXPos(float num), SetYPos(float num), SetZPos(float num);
	float		GetXPos(), GetYPos(), GetZPos();

	void		SetXRot(float num), SetYRot(float num), SetZRot(float num);
	float		GetXRot(), GetYRot(), GetZRot();

	void		SetXScale(float num), SetYScale(float num), SetZScale(float num);
	float		GetXScale(), GetYScale(), GetZScale();

	void		IncXPos(float num), IncYPos(float num), IncZPos(float num);
	void		DecXPos(float num), DecYPos(float num), DecZPos(float num);

	void		IncXRot(float num), IncYRot(float num), IncZRot(float num);
	void		DecXRot(float num), DecYRot(float num), DecZRot(float num);

	void		IncXScale(float num), IncYScale(float num), IncZScale(float num);
	void		DecXScale(float num), DecYScale(float num), DecZScale(float num);

private:
	
	//Mesh properties
	Mesh*			m_meshModel;
	Shader*			m_meshShader;
	Texture*		m_meshDiffuseTexture;
	Texture*		m_meshSpecularTexture;

	GLuint m_VBO[3];

	//Space
	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;

	glm::mat4 m_mMat;		//Model
	glm::mat4 m_vMat;		//View
	glm::mat4 m_tMat;		//Translation
	glm::mat4 m_rMat;		//Rotation
	glm::mat4 m_sMat;		//Scale
	glm::mat4 m_invTrMat;	//Inverse transpose
	

	//Cached other classes
	LightManager* m_localLightManager;


};

