#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

//#include "EngineStatics.h"

class Input
{
public:
	Input();
	~Input();

	static void keyCALLBACK(GLFWwindow* window, int key, int scancode, int action, int mode);
	static bool getKeyPressed(int key);

	static void mouseCALLBACK(GLFWwindow* window, double xPos, double yPos);
	static void getMouseMoved(double& xMouse, double& yMouse);


private:

	//Keyboard
	static bool keys[1024];

	//Mouse
	static GLfloat lastX;
	static GLfloat lastY;
	static bool firstMouse;
	static GLfloat xOffset;
	static GLfloat yOffset;

};

