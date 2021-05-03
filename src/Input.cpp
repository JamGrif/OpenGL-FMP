#include "Input.h"

#include <iostream>

#include "EngineStatics.h"

//Keyboard
bool Input::keys[1024];
int Input::lastKey = 0;

//Mouse
double Input::lastX = 0;
double Input::lastY = 0;
bool Input::firstMouse = true;
double Input::xOffset = 0;
double Input::yOffset = 0;


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

/// <summary>
/// Function is called everytime application detects keyboard input
/// </summary>
/// <param name="window">The window that has input</param>
/// <param name="key">Key pressed</param>
/// <param name="scancode"></param>
/// <param name="action"></param>
/// <param name="mode"></param>
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
           // lastKey = key;
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            lastKey = 0;
            keys[key] = false;
        }
    }
}

/// <summary>
/// Returns whether the specified key is pressed or not
/// </summary>
/// <param name="key">Specified key to query</param>
/// <returns>If the key is pressed or not</returns>
bool Input::getKeyPressed(int key)
{
    if (keys[key])
    {
        
        return true;
    }
	return false;
}

bool Input::getKeyPressedOnce(int key)
{
    if (keys[key] && key != lastKey)
    {
        lastKey = key;
        return true;
    }
    return false;
}

/// <summary>
/// Function is called everytime application detects mouse input
/// </summary>
/// <param name="window">The window that has input</param>
/// <param name="xPos">New mouse X position</param>
/// <param name="yPos">New mouse Y position</param>
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

