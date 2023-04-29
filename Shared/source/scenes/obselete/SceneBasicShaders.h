#pragma once
#include "Scene.h"

/*
* Very basic triangle draw. To learn about the basics of opengl.
*/
class SceneBasicShaders : public Scene
{
	// GPU data
	unsigned int VBO, EBO, VAO;
	unsigned int shaderProgram;

	void InitBuffers();

public:
	SceneBasicShaders();
	~SceneBasicShaders();

	void Init();
	void Update(GLFWwindow* window, float deltaTime);
	void Draw();
};