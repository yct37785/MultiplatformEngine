#include "SceneBasicRectangle.h"

SceneBasicRectangle::SceneBasicRectangle()
{
	VBO = VAO = EBO = shaderProgram = 0;
}

SceneBasicRectangle::~SceneBasicRectangle()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);
}

void SceneBasicRectangle::InitBuffers()
{
	float vertices[] = {
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	// VAO (stores all attributes of the following GL_ARRAY_BUFFER
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// EBO
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);	// bind the above created buffer object to this target
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);	// copy user defined data to the bounded buffer

	// tell OpenGL how it should intepret the vertex data (per vertex attirbute)
	// param 1: specify which vertex attrib to configure: layout (location = 0)
	// param 2: size of vertex attribute (3 values)
	// param 3: data type
	// param 4: specify if data is to be normalized (not relevant)
	// param 5: stride
	// param 6: offset of where this data of where vertex data begins in the buffer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); // enable the vertx attrib and give vertex attrib location

	// unbind
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);	// !unbind last! Or else it will store the unbinded EBO (glBindBuffer GL_ELEMENT_ARRAY_BUFFER as 0)!
}

void SceneBasicRectangle::Init()
{
	loadAndLinkShaders("../Shaders/basic/vertexshader.cpp", "../Shaders/basic/fragmentshader.cpp", shaderProgram);
	InitBuffers();

	// use shader
	glUseProgram(shaderProgram);
}

void SceneBasicRectangle::Update(GLFWwindow* window, float deltaTime)
{
}

void SceneBasicRectangle::Draw()
{
	// bind VAO
	glBindVertexArray(VAO);

	// bind EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	// draws the primitves with indices (stored in EBO)
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
