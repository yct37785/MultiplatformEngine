#pragma once
#include "Scene.h"

/*
* Very basic triangle draw. To learn about the basics of opengl.
*/
class SceneBasicTriangle : public Scene
{
	// GPU data
	unsigned int VBO, VAO;
	unsigned int shaderProgram;

	void InitBuffers();

public:
	SceneBasicTriangle();
	~SceneBasicTriangle();

	void Init();
	void Update(GLFWwindow* window, float deltaTime);
	void Draw();
};