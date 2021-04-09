#include "Application.h"

Application::Application()
	:m_defaultWindowWidth(1280), m_defaultWindowHeight(720), m_currentWindowWidth(0), m_currentWindowHeight(0), m_aspectRatio(0.0),
	m_appWindow(nullptr), m_projMatrix{ 1.0f }, m_appVAO(0), m_demoScene(nullptr), m_input(nullptr),
	m_deltaTime(0.0f), m_lastFrame(0.0f)

{
	std::cout << "Application Initialized" << std::endl;
}

Application::~Application()
{
	std::cout << "Application Destroyed" << std::endl;

	delete m_demoScene;
	m_demoScene = nullptr;

	delete m_input;
	m_input = nullptr;

	glDeleteVertexArrays(1, &m_appVAO);

	glfwDestroyWindow(m_appWindow);
	glfwTerminate();
}

int Application::appInit()
{
	//Initalize GLFW

	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	m_appWindow = glfwCreateWindow(m_defaultWindowWidth, m_defaultWindowHeight, "OpenGL - Jamie", NULL, NULL);

	//Set Icon
	GLFWimage images[1];
	images[0].pixels = stbi_load("res/Icon.jpg", &images[0].width, &images[0].height, 0, 4); //rgba channels 
	glfwSetWindowIcon(m_appWindow, 1, images);
	stbi_image_free(images[0].pixels);

	glfwMakeContextCurrent(m_appWindow);
	EngineStatics::setAppWindow(m_appWindow);

	std::cout << glGetString(GL_VENDOR) << std::endl;
	std::cout << glGetString(GL_RENDERER) << std::endl;
	std::cout << glGetString(GL_VERSION) << std::endl << std::endl;

	//Initalize GLEW

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		exit(EXIT_FAILURE);
	}

	glfwSwapInterval(1);
	glClearColor(0.0, 0.0, 0.0, 1.0); //Sets clear colour

	//Build projection matrix
	glfwGetFramebufferSize(m_appWindow, &m_currentWindowWidth, &m_currentWindowHeight);
	glViewport(0, 0, m_currentWindowWidth, m_currentWindowHeight);
	m_aspectRatio = (float)m_currentWindowWidth / (float)m_currentWindowHeight;
	m_projMatrix = glm::perspective(1.0472f, m_aspectRatio, 0.01f, 1000.0f); //1.0472 = 60 degrees
	EngineStatics::setProjectionMatrix(&m_projMatrix);

	//VAO
	glGenVertexArrays(1, &m_appVAO);
	glBindVertexArray(m_appVAO);

	//Depth Buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	//Backface Culling
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	glEnable(GL_MULTISAMPLE);


	//Create Input object
	m_input = new Input();

	//Create first scene
	m_demoScene = new Scene();
	m_demoScene->initScene();

	return 0;
}

void Application::appLoop()
{
	while (!glfwWindowShouldClose(m_appWindow))
	{
		//Delta time
		GLfloat currentFrame = glfwGetTime();
		m_deltaTime = currentFrame - m_lastFrame;
		m_lastFrame = currentFrame;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clears the screen buffers
		glfwPollEvents();

		m_demoScene->updateScene();

		glfwSwapBuffers(m_appWindow);

	}
	
}

void Application::windowResizeCALLBACK(GLFWwindow* window, int newWidth, int newHeight)
{

}
