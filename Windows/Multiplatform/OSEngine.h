#pragma once
#include <engine/Engine.h>

/*
* OS dependant definitions. Load the respective .cpp definitions depending on your OS.
*/
class OSEngine
{
	bool inputList[INPUT_TOTAL];
	float deltaTime, lastFrame;
	static OSEngine* osEngine;
	static GLFWwindow* window;

private:
	OSEngine();

	// init OpenGL/glfw
	void initGlfw();
	int createWindow();
	int initGlad();
	void createViewport();
	void initInput();
	void initOpenGLSettings();

	// render loop
	void renderLoop();

	// exit
	int terminateOpenGL();

	// event handling
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void processInput(GLFWwindow* window);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

public:
	~OSEngine();
	static OSEngine* instance();

	void Init();
	void Run();
};

