#pragma once

#include "Shader.h"

enum
{
	screen_Default = 1,
	screen_Inverse,
	END_OF_FILTER_ENUM
};

class Framebuffer
{
public:
	Framebuffer(bool multisampled);
	~Framebuffer();

	void draw();

	void bindFramebuffer();
	void unbindFramebuffer();

	void setFrameFilter(int index);

	unsigned int getFBO();

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

	Shader* m_screenShader;
	unsigned int			m_quadVBO;

	int m_screenFilter;

};

