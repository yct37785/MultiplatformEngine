#include "SceneBasicLighting.h"

SceneBasicLighting::SceneBasicLighting()
{
}

SceneBasicLighting::~SceneBasicLighting()
{
}

void SceneBasicLighting::Init()
{
	// any flags please set here

	// load shaders
	shaderProgram.Init("../Shaders/lightingBasic/vertexshader.cpp", "../Shaders/lightingBasic/fragmentshader.cpp");
	lightSourceShaderProgram.Init("../Shaders/lightSource/vertexshader.cpp", "../Shaders/lightSource/fragmentshader.cpp");
	shaderProgram.activateShader();	// activate to set uniforms

	InitObjects();
	camera.Init();

	// set uniform
	shaderProgram.setUniform1f("alphaValue", 1.f);
	// tell opengl which sampler belongs to which texture (GL_TEXTURE_2D binded order)
	shaderProgram.setUniform1i("textureSampler2D", 0);
}

void SceneBasicLighting::InitObjects()
{
	// objects
	cubes[0].Init(MESH_CUBE, TEX_BRICKWALL, glm::vec3(1.f, 1.f, 1.f), glm::vec3(2.f, 0.f, 0.f), 5.f, -10.f);
	cubes[1].Init(MESH_CUBE, TEX_METAL, glm::vec3(1.f, 1.f, 1.f), glm::vec3(-2.f, 0.f, 0.f), 0.f, 0.f);
	cubes[2].Init(MESH_CUBE, TEX_WOODTILES, glm::vec3(1.f, 1.f, 1.f), glm::vec3(0.f, 2.f, 0.5f), 50.f, 20.f);
	cubes[3].Init(MESH_SPHERE, TEX_METAL, glm::vec3(1.5f, 1.5f, 1.5f), glm::vec3(0.f, 0.f, -3.f), 45.f, 0.f);
	cubes[4].Init(MESH_CUBE, TEX_METAL, glm::vec3(2.f, 2.f, 2.f), glm::vec3(0.f, -2.f, 0.f), 20.f, 160.f);

	// light
	lightBulb0.Init(MESH_SPHERE, TEX_BRICKWALL, glm::vec3(0.15f, 0.15f, 0.15f), glm::vec3(0.f, 0.f, 0.0f), 0.f, 0.f);
	light_0_color = glm::vec3(1.f, 1.f, 1.f);
}

void SceneBasicLighting::Update(GLFWwindow* window, float deltaTime)
{
	// last
	camera.UpdateInput(window, deltaTime);
	UpdateTransformation(deltaTime);
}

void SceneBasicLighting::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	camera.mouse_callback(window, xpos, ypos);
}

void SceneBasicLighting::UpdateTransformation(float deltaTime)
{
	// spin
	cubes[0].rotate(deltaTime * 12.f, 0.f);
	cubes[2].rotate(deltaTime * 12.f, deltaTime * 6.f);
	cubes[4].rotate(0.f, deltaTime * 20.f);

	// model
	for (int i = 0; i < total_gameObjects; ++i)
	{
		cubes[i].UpdateTransform();
	}
	lightBulb0.UpdateTransform();

	// view
	UpdateViewMatrix();

	// proj
	projection = glm::perspective(glm::radians(45.f), 800.f / 600.f, 0.1f, 100.f);
}

void SceneBasicLighting::UpdateViewMatrix()
{
	// lookat matrix
	// we create a matrix with the above 3 perpendicular axes + translation vector
	// this matrix can transform any vector to the view coord space just by multiplying with it
	// more details: https://learnopengl.com/Getting-started/Camera
	camera.genLookUpMatrix(view);
}

void SceneBasicLighting::Draw()
{
	// use shader
	shaderProgram.activateShader();
	shaderProgram.setUniformMatrix4fv("view", view);
	shaderProgram.setUniformMatrix4fv("projection", projection);
	shaderProgram.setUniform3fv("light_0_color", light_0_color);
	shaderProgram.setUniform3fv("light_0_pos", lightBulb0.getPos());
	shaderProgram.setUniform3fv("eyePos", camera.getPos());

	// draw
	for (int i = 0; i < total_gameObjects; ++i)
	{
		cubes[i].Draw(shaderProgram);
	}

	// use shader
	lightSourceShaderProgram.activateShader();
	lightSourceShaderProgram.setUniformMatrix4fv("view", view);
	lightSourceShaderProgram.setUniformMatrix4fv("projection", projection);

	lightBulb0.Draw(lightSourceShaderProgram);
}