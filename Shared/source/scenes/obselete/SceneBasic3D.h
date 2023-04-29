#pragma once
#include "Scene.h"

/*
* Very basic triangle draw. To learn about the basics of opengl.
*/
class SceneBasic3D : public Scene
{
	// an object
	unsigned int VBO, EBO, VAO;
	glm::vec3 pos, scale;

	// texture
	unsigned int texture, texture_1;

	// shader
	unsigned int shaderProgram;

	// transformation
	glm::mat4 model, view, projection;

	void InitBuffers();
	void InitTextures();
	void InitObjects();
	void UpdateTransformation();

public:
	SceneBasic3D();
	~SceneBasic3D();

	void Init();
	void Update(GLFWwindow* window, float deltaTime);
	void Draw();
};