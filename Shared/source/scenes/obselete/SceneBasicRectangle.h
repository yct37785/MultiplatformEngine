#pragma once
#include "Scene.h"

/*
* Very basic triangle draw. To learn about the basics of opengl.
*/
class SceneBasicRectangle : public Scene
{
	// GPU data
	unsigned int VBO, EBO, VAO;
	unsigned int shaderProgram;

	void InitBuffers();

public:
	SceneBasicRectangle();
	~SceneBasicRectangle();

	void Init();
	void Update(GLFWwindow* window, float deltaTime);
	void Draw();
};