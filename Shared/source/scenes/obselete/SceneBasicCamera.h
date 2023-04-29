#pragma once
#include "Scene.h"

/*
* Very basic triangle draw. To learn about the basics of opengl.
*/
class SceneBasicCamera : public Scene
{
	// an object
	unsigned int VBO, EBO, VAO;
	glm::vec3 pos, scale;

	// texture
	unsigned int texture, texture_1;

	// shader
	unsigned int shaderProgram;

	// camera
	Camera camera;

	// transformation
	glm::mat4 model, view, projection;

	void InitBuffers();
	void InitTextures();
	void InitObjects();
	void UpdateTransformation();
	void UpdateViewMatrix();

public:
	SceneBasicCamera();
	~SceneBasicCamera();

	void Init();
	void Update(GLFWwindow* window, float deltaTime);
	void Draw();
	
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
};