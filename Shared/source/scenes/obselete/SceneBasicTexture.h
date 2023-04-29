#pragma once
#include "Scene.h"

/*
* Very basic triangle draw. To learn about the basics of opengl.
*/
class SceneBasicTexture : public Scene
{
	// GPU data
	unsigned int VBO, EBO, VAO;
	unsigned int texture, texture_1;
	unsigned int shaderProgram;

	void InitBuffers();

public:
	SceneBasicTexture();
	~SceneBasicTexture();

	void Init();
	void Update(GLFWwindow* window, float deltaTime);
	void Draw();
};