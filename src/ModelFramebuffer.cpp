#include "ModelFramebuffer.h"

ModelFramebuffer::ModelFramebuffer(glm::vec3 position, glm::vec3 rotation)
	:Model(position, rotation)
{
	std::cout << "creating framebuffer" << std::endl;

	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo); //By binding to a GL_FRAMEBUFFER, all read and write framebuffer operations will be on newly bounded framebuffer

	glGenTextures(1, &m_frameColourTexture);
	glBindTexture(GL_TEXTURE_2D, m_frameColourTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1280, 720, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_frameColourTexture, 0);

	glGenRenderbuffers(1, &m_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1280, 720);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Problem creating framebuffer! ---------------------------------------------------------- " << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	setShaderTwo("res/shaders/framebuffer-vertex.glsl", "res/shaders/framebuffer-fragment.glsl");

	glGenBuffers(1, &m_quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
}

ModelFramebuffer::~ModelFramebuffer()
{

	glDeleteFramebuffers(1, &m_fbo);
}

void ModelFramebuffer::drawPassOne()
{
	if (m_modelMesh == nullptr || m_modelShaderPassOne == nullptr)
	{
		return;
	}
}

void ModelFramebuffer::drawPassTwo()
{
	if (m_modelShaderPassTwo == nullptr)
	{
		return;
	}

	m_modelShaderPassTwo->Bind();

	m_modelShaderPassTwo->setUniform1i("screenTexture", 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_frameColourTexture);

	glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);

	glDisable(GL_DEPTH_TEST);


	

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));



	glDrawArrays(GL_TRIANGLES, 0, 6);

	m_modelShaderPassTwo->Unbind();
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ModelFramebuffer::bindFramebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

void ModelFramebuffer::unbindFramebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
