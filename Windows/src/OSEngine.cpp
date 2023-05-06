#include "OSEngine.h"
OSEngine* OSEngine::osEngine = NULL;
GLFWwindow* OSEngine::window = NULL;

void OSEngine::initGlfw()
{
	glfwInit();
	// set OpenGL ver
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	// core profile: excudes backwards-compatible features we no longer need
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

int OSEngine::createWindow()
{
	window = glfwCreateWindow(windowsWidth, windowsHeight, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		LogWarn("Failed to create GLFW window");
		terminateOpenGL();
		return 0;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);	// register callback for on windows resize event
	return 1;
}

int OSEngine::initGlad()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		LogWarn("Failed to initialize GLAD");
		return 0;
	}
	return 1;
}

void OSEngine::createViewport()
{
	glViewport(0, 0, windowsWidth, windowsHeight);
}

void OSEngine::initInput()
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
}

void OSEngine::initOpenGLSettings()
{
	// opengl rendering configurations
	// depth ordering on OpenGL
	glEnable(GL_DEPTH_TEST);
	// alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// cull face (discard unseen faces, save 50% on fragment shader runs)
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
}

void OSEngine::renderLoop()
{
	// OSEngine vars
	lastFrame = glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{
		// delta time
		float currFrame = glfwGetTime();
		deltaTime = currFrame - lastFrame;
		lastFrame = currFrame;

		// input
		processInput(window);

		// engine update
		Engine::instance()->Update(inputList, deltaTime);
		Engine::instance()->Draw();

		// read: double buffers to prevent flickering issues due to physical constraints of drawing a buffer to screen
		// resulting in flickering
		glfwSwapBuffers(window);

		// check if events triggered (inputs etc), then updates windows state and activate callbacks
		glfwPollEvents();
	}
}

int OSEngine::terminateOpenGL()
{
	glfwTerminate();
	return 1;
}

void OSEngine::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	Engine::instance()->onWindowSizeUpdate(width, height);
}

void OSEngine::processInput(GLFWwindow* window)
{
	// reset all keys
	for (int i = 0; i < INPUT_TOTAL; ++i)
	{
		inputList[i] = false;
	}

	// set input
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		inputList[INPUT_FWD] = true;
	} if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		inputList[INPUT_BWD] = true;
	} if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		inputList[INPUT_LEFT] = true;
	} if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		inputList[INPUT_RIGHT] = true;
	} if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
		inputList[INPUT_UP] = true;
	} if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		inputList[INPUT_DW] = true;
	} if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		inputList[INPUT_ENABLE_WIREFRAME] = true;
	} if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		inputList[INPUT_DISABLE_WIREFRAME] = true;
	} if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		inputList[INPUT_QUIT] = true;
		glfwSetWindowShouldClose(window, true);
	}
}

void OSEngine::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	Engine::instance()->onMouseUpdate(xpos, ypos);
}

OSEngine::OSEngine()
{
	deltaTime = lastFrame = 0.f;

	for (int i = 0; i < INPUT_TOTAL; ++i)
	{
		inputList[i] = false;
	}
}

OSEngine::~OSEngine()
{
	delete Engine::instance();
	terminateOpenGL();
}

OSEngine* OSEngine::instance()
{
	if (!osEngine)
	{
		osEngine = new OSEngine();
	}
	return osEngine;
}

void OSEngine::Init()
{
	initGlfw();
	createWindow();
	initGlad();
	createViewport();
	initInput();
	initOpenGLSettings();
	LogInfo("OSEngine initialized");
	Engine::instance()->Init();
}

void OSEngine::Run()
{
	renderLoop();
}