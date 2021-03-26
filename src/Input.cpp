#include "Input.h"

#include "EngineStatics.h"

//Keyboard
bool Input::keys[1024];

//Mouse
GLfloat Input::lastX = 0;
GLfloat Input::lastY = 0;
bool Input::firstMouse = true;
GLfloat Input::xOffset = 0;
GLfloat Input::yOffset = 0;


Input::Input()
{
	std::cout << "Input Initialized" << std::endl;
	 
	glfwSetKeyCallback(EngineStatics::getAppWindow(), keyCALLBACK);
	glfwSetCursorPosCallback(EngineStatics::getAppWindow(), mouseCALLBACK);

    glfwSetInputMode(EngineStatics::getAppWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED); //Hide cursor during runtime

}

Input::~Input()
{
	std::cout << "Input Destroyed" << std::endl;
}

void Input::keyCALLBACK(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
        }
    }
}

bool Input::getKeyPressed(int key)
{
    if (keys[key])
    {
        //std::cout << "key true" << std::endl;
        return true;
    }
	return false;
}

void Input::mouseCALLBACK(GLFWwindow* window, double xPos, double yPos)
{
    //std::cout << "mousecallback" << std::endl;
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    xOffset = xPos - lastX;
    yOffset = lastY - yPos; //Reversed as Y coordinates go from bottom to left

    lastX = xPos;
    lastY = yPos;
}

void Input::getMouseMoved(double& xMouse, double& yMouse)
{
    xMouse = xOffset;
    yMouse = yOffset;

    xOffset = 0;
    yOffset = 0;

}

