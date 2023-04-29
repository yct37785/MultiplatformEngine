#include "SceneBasicTransform.h"

SceneBasicTransform::SceneBasicTransform()
{
	VBO = VAO = EBO = shaderProgram = 0;
	texture = texture_1 = 0;
}

SceneBasicTransform::~SceneBasicTransform()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);
}

void SceneBasicTransform::InitBuffers()
{
	float vertices[] = {
		// position			// color		  // tex coord
		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top right
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f // top left 
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// tex attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// texture
	generateTexture("../Assets/textures/brick-seamless.jpg", texture);
	generateTexture("../Assets/textures/Akko Kagari.png", texture_1);

	// unbind
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);	// !unbind last! Or else it will store the unbinded EBO (glBindBuffer GL_ELEMENT_ARRAY_BUFFER as 0)!
}

void SceneBasicTransform::Init()
{
	// any flags please set here
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);

	// load shaders
	loadAndLinkShaders("../Shaders/transformBasic/vertexshader.cpp", "../Shaders/transformBasic/fragmentshader.cpp", shaderProgram);
	InitBuffers();

	// use shader
	glUseProgram(shaderProgram);

	// set uniform
	glUniform1f(glGetUniformLocation(shaderProgram, "alphaValue"), 1.0f);
	// tell opengl which sampler belongs to which texture (GL_TEXTURE_2D binded order)
	glUniform1i(glGetUniformLocation(shaderProgram, "textureSampler2D"), 0);
	glUniform1i(glGetUniformLocation(shaderProgram, "textureSampler2D_1"), 1);
}

void SceneBasicTransform::UpdateTransformation()
{
	transformMat = glm::mat4(1.0f);
	transformMat = glm::translate(transformMat, glm::vec3(0.5f, -0.5f, 1.0f));
	transformMat = glm::rotate(transformMat, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
}

void SceneBasicTransform::Update(GLFWwindow* window, float deltaTime)
{
	UpdateTransformation();
}

void SceneBasicTransform::Draw()
{
	// bind texture
	glActiveTexture(GL_TEXTURE0); // activate the texture unit first (sampler2D in fragment shader)
	glBindTexture(GL_TEXTURE_2D, texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture_1);

	// bind VAO
	glBindVertexArray(VAO);

	// bind EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	// uniforms (transformation)
	unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transformMat");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformMat));

	// draws the primitves with indices (stored in EBO)
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
