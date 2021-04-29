#pragma once

#include "Model.h"

class ModelFramebuffer:
	public Model
{
public:
	ModelFramebuffer(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f));
	~ModelFramebuffer();

	void drawPassOne() override;
	void drawPassTwo() override;

	void bindFramebuffer();
	void unbindFramebuffer();

private:

	float quadVertices[24] =
	{
		//Pos		//Texcoords
		 -1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	unsigned int			m_fbo;
	unsigned int			m_frameColourTexture;
	unsigned int			m_rbo;

	unsigned int			m_quadVBO;

};

