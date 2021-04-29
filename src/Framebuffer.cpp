#include "Framebuffer.h"

#include <iostream>

Framebuffer::Framebuffer(bool multisampled)
	:m_screenFilter(1)
{
	std::cout << "creating framebuffer" << std::endl;

	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo); //By binding to a GL_FRAMEBUFFER, all read and write framebuffer operations will be on newly bounded framebuffer

	glGenTextures(1, &m_frameColourTexture);
	if (!multisampled)
	{
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
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_frameColourTexture);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, 1280, 720, GL_TRUE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_frameColourTexture, 0);

		glGenRenderbuffers(1, &m_rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, 1280, 720);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);


	}
	

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Problem creating framebuffer! ---------------------------------------------------------- " << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_screenShader = ShaderManager::loadShader("res/shaders/framebuffer-vertex.glsl", "res/shaders/framebuffer-fragment.glsl");

	glGenBuffers(1, &m_quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
}

Framebuffer::~Framebuffer()
{

	glDeleteFramebuffers(1, &m_fbo);
}



void Framebuffer::draw()
{
	if (m_screenShader == nullptr)
	{
		return;
	}

	m_screenShader->Bind();

	m_screenShader->setUniform1i("screenTexture", 0);
	m_screenShader->setUniform1i("screenFilter", m_screenFilter);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_frameColourTexture);

	glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);

	glDisable(GL_DEPTH_TEST);


	

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));



	glDrawArrays(GL_TRIANGLES, 0, 6);

	m_screenShader->Unbind();
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Framebuffer::bindFramebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glEnable(GL_DEPTH_TEST);
}

void Framebuffer::unbindFramebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glClear(GL_COLOR_BUFFER_BIT);
}

void Framebuffer::setFrameFilter(int index)
{
	if (index >= END_OF_FILTER_ENUM || index <= 0)
	{
		return;
	}

	m_screenFilter = index;
}

unsigned int Framebuffer::getFBO()
{
	return m_fbo;
}
