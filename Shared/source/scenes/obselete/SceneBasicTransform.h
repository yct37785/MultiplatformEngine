#pragma once
#include "Scene.h"

/*
* Very basic triangle draw. To learn about the basics of opengl.
*/
class SceneBasicTransform : public Scene
{
	// an object
	unsigned int VBO, EBO, VAO;
	glm::mat4 transformMat;

	// texture
	unsigned int texture, texture_1;

	// shader
	unsigned int shaderProgram;

	void InitBuffers();
	void UpdateTransformation();

public:
	SceneBasicTransform();
	~SceneBasicTransform();

	void Init();
	void Update(GLFWwindow* window, float deltaTime);
	void Draw();
};