#include "SceneBasicShaders.h"

SceneBasicShaders::SceneBasicShaders()
{
	VBO = VAO = EBO = shaderProgram = 0;
}

SceneBasicShaders::~SceneBasicShaders()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);
}

void SceneBasicShaders::InitBuffers()
{
	float vertices[] = {
		// position			// color
		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom left
		-0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f   // top left 
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

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// unbind
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);	// !unbind last! Or else it will store the unbinded EBO (glBindBuffer GL_ELEMENT_ARRAY_BUFFER as 0)!
}

void SceneBasicShaders::Init()
{
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	// cout << "Maximum nr of vertex attributes supported: " << nrAttributes << endl;

	loadAndLinkShaders("../Shaders/normal/vertexshader.cpp", "../Shaders/normal/fragmentshader.cpp", shaderProgram);
	InitBuffers();

	// use shader
	glUseProgram(shaderProgram);

	// set uniform
	glUniform1f(glGetUniformLocation(shaderProgram, "alphaValue"), 0.25f);
}

void SceneBasicShaders::Update(GLFWwindow* window, float deltaTime)
{
}

void SceneBasicShaders::Draw()
{
	// bind VAO
	glBindVertexArray(VAO);

	// bind EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	// draws the primitves with indices (stored in EBO)
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
