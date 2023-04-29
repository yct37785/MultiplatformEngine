#include "SceneBasicCamera.h"

SceneBasicCamera::SceneBasicCamera()
{
	VBO = VAO = EBO = shaderProgram = 0;
	texture = texture_1 = 0;
}

SceneBasicCamera::~SceneBasicCamera()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);
}

void SceneBasicCamera::Init()
{
	// any flags please set here
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);

	// load shaders
	loadAndLinkShaders("../Shaders/3dBasic/vertexshader.cpp", "../Shaders/3dBasic/fragmentshader.cpp", shaderProgram);
	InitBuffers();
	InitTextures();
	InitObjects();
	camera.Init();

	// use shader
	glUseProgram(shaderProgram);

	// set uniform
	glUniform1f(glGetUniformLocation(shaderProgram, "alphaValue"), 1.f);
	// tell opengl which sampler belongs to which texture (GL_TEXTURE_2D binded order)
	glUniform1i(glGetUniformLocation(shaderProgram, "textureSampler2D"), 0);
	glUniform1i(glGetUniformLocation(shaderProgram, "textureSampler2D_1"), 1);
}

void SceneBasicCamera::InitBuffers()
{
	/*
		top view:
			   face 3
			    ----
		face 2 |    | face 4
				----
			   face 1

	    side view:
			face 6
			|    |
			face 5
	*/
	float vertices[] = {
		// position			// color		  // tex coord
		// face 1
		 0.5f,  0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,	// 0
		 0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,	// 1
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,	// 2
		-0.5f,  0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,	// 3
		// face 2
		-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,	// 4
		-0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,	// 5
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,	// 6
		-0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,	// 7
		// face 3
		-0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,	// 8
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,	// 9
		 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,	// 10
		 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,	// 11
		// face 4
		 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,	// 12
		 0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,	// 13
		 0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,	// 14
		 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,	// 15
		// face 5
		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,	// 19
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,	// 18
		 0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,	// 17
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,	// 16
		// face 6
		 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,	// 20
		 0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,	// 21
		-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,	// 22
		-0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f	// 23
	};
	
	// always draw our vertices in a CW direction
	unsigned int indices[6 * 6];
	for (int i = 0, j = 0; i < 6 * 6; i += 6, j += 4)
	{
		// triangle 1
		indices[i + 0] = j + 0;
		indices[i + 1] = j + 1;
		indices[i + 2] = j + 3;
		// triangle 2
		indices[i + 3] = j + 1;
		indices[i + 4] = j + 2;
		indices[i + 5] = j + 3;
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

	// unbind
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);	// !unbind last! Or else it will store the unbinded EBO (glBindBuffer GL_ELEMENT_ARRAY_BUFFER as 0)!
}

void SceneBasicCamera::InitTextures()
{
	generateTexture("../Assets/textures/brick-seamless.jpg", texture);
	generateTexture("../Assets/textures/Akko Kagari.png", texture_1);
}

void SceneBasicCamera::InitObjects()
{
	// objects
	pos = glm::vec3(0.f, 0.f, 0.f);
	scale = glm::vec3(1.f, 1.f, 1.f);
}

void SceneBasicCamera::Update(GLFWwindow* window, float deltaTime)
{
	// last
	camera.UpdateInput(window, deltaTime);
	UpdateTransformation();
}

void SceneBasicCamera::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	camera.mouse_callback(window, xpos, ypos);
}

void SceneBasicCamera::UpdateTransformation()
{
	// model
	model = glm::mat4(1.f);
	model = glm::translate(model, pos);
	// model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.f), glm::vec3(0.f, 1.f, 0.f));
	model = glm::scale(model, scale);

	// view
	UpdateViewMatrix();

	// proj
	projection = glm::perspective(glm::radians(45.f), 800.f / 600.f, 0.1f, 100.f);
}

void SceneBasicCamera::UpdateViewMatrix()
{
	// lookat matrix
	// we create a matrix with the above 3 perpendicular axes + translation vector
	// this matrix can transform any vector to the view coord space just by multiplying with it
	// more details: https://learnopengl.com/Getting-started/Camera
	camera.genLookUpMatrix(view);
}

void SceneBasicCamera::Draw()
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
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	// draws the primitves with indices (stored in EBO)
	glDrawElements(GL_TRIANGLES, 6 * 6, GL_UNSIGNED_INT, 0);
}