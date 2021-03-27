#pragma once


#include "ModelLoader.h"
#include "Shader.h"
#include "Utility.h"
#include "EngineStatics.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>

#include <SOIL2.h>


class Mesh
{
public:
	Mesh(const char* meshFilePath);
	~Mesh();

	void initMesh(const char* meshFilePath);
	void updateMesh();
	void drawMesh();

	void loadDiffuseTexture(const char* texturePath);
	void loadSpecularTexture(const char* texturePath);

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
	ImportedModel* meshModel;
	Shader* meshShader;
	GLuint meshDiffuseTexture;
	GLuint meshSpecularTexture;

	GLuint VBO[3];

	//Space
	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;

	glm::mat4 mMat;     //Model
	glm::mat4 vMat;     //View
	glm::mat4 tMat;     //Translation
	glm::mat4 rMat;     //Rotation
	glm::mat4 sMat;		//Scale
	glm::mat4 invTrMat; //Inverse transpose
	

	//Uniforms
	//Vertex Shader
	GLuint mLoc, vLoc, projLoc;

	//Fragment Shader
	GLuint lightAmbLoc, lightDiffLoc, lightSpecLoc, lightPosLoc, viewPosLoc;


};

