#include "SceneBasicTriangle.h"

SceneBasicTriangle::SceneBasicTriangle()
{
	VBO = VAO = shaderProgram = 0;
}

SceneBasicTriangle::~SceneBasicTriangle()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
}

void SceneBasicTriangle::InitBuffers()
{
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, // left  
		0.5f, -0.5f, 0.0f, // right 
		0.0f,  0.5f, 0.0f  // top   
	};

	// VAO (stores all attributes of the following GL_ARRAY_BUFFER
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

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
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void SceneBasicTriangle::Init()
{
	loadAndLinkShaders("../Shaders/basic/vertexshader.cpp", "../Shaders/basic/fragmentshader.cpp", shaderProgram);
	InitBuffers();

	// use shader
	glUseProgram(shaderProgram);
}

void SceneBasicTriangle::Update(GLFWwindow* window, float deltaTime)
{
}

void SceneBasicTriangle::Draw()
{
	// bind VAO
	glBindVertexArray(VAO);

	// draws the primitves using currently active shader, vertex attrib configurations and VBO (via VAO)
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
