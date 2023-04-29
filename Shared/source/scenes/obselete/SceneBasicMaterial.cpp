#include "SceneBasicMaterial.h"

SceneBasicMaterial::SceneBasicMaterial()
{
}

SceneBasicMaterial::~SceneBasicMaterial()
{
}

void SceneBasicMaterial::Init()
{
	// any flags please set here

	InitObjects();
	camera.Init();

	// activate to set uniforms
	ShaderManager::instance()->useShader(SHADER_MATERIAL_BASIC);

	// set uniform
	ShaderManager::instance()->setUniform1f(SHADER_MATERIAL_BASIC, "alphaValue", 1.f);
	// tell opengl which sampler belongs to which texture (GL_TEXTURE_2D binded order)
	ShaderManager::instance()->setUniform1i(SHADER_MATERIAL_BASIC, "textureSampler2D", 0);
}

void SceneBasicMaterial::InitObjects()
{
	// objects
	cubes[0].Init(MESH_CUBE, TEX_BRICKWALL, MAT_PLASTIC, glm::vec3(1.f, 1.f, 1.f), glm::vec3(2.f, 0.f, 0.f), 5.f, -10.f);
	cubes[1].Init(MESH_CUBE, TEX_CONTAINER_FACE, MAT_PLASTIC, glm::vec3(1.f, 1.f, 1.f), glm::vec3(-2.f, 0.f, 0.f), 0.f, 0.f);
	cubes[2].Init(MESH_CUBE, TEX_WOODTILES, MAT_PLASTIC, glm::vec3(1.f, 1.f, 1.f), glm::vec3(0.f, 2.f, 0.5f), 50.f, 20.f);
	cubes[3].Init(MESH_SPHERE, TEX_METAL, MAT_NONE, glm::vec3(1.5f, 1.5f, 1.5f), glm::vec3(0.f, 0.f, -3.f), 45.f, 0.f);
	cubes[4].Init(MESH_CUBE, TEX_METAL, MAT_PLASTIC, glm::vec3(2.f, 2.f, 2.f), glm::vec3(0.f, -2.f, 0.f), 20.f, 160.f);

	// light
	lightBulb0.Init(MESH_SPHERE, TEX_BRICKWALL, MAT_NONE, glm::vec3(0.15f, 0.15f, 0.15f), glm::vec3(0.f, 0.f, 0.0f), 0.f, 0.f);
	light_0_color = glm::vec3(1.f, 1.f, 1.f);
}

void SceneBasicMaterial::Update(GLFWwindow* window, float deltaTime)
{
	// last
	camera.UpdateInput(window, deltaTime);
	UpdateTransformation(deltaTime);
}

void SceneBasicMaterial::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	camera.mouse_callback(window, xpos, ypos);
}

void SceneBasicMaterial::UpdateTransformation(float deltaTime)
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

void SceneBasicMaterial::UpdateViewMatrix()
{
	// lookat matrix
	// we create a matrix with the above 3 perpendicular axes + translation vector
	// this matrix can transform any vector to the view coord space just by multiplying with it
	// more details: https://learnopengl.com/Getting-started/Camera
	camera.genLookUpMatrix(view);
}

void SceneBasicMaterial::Draw()
{
	// use shader
	ShaderManager::instance()->useShader(SHADER_MATERIAL_BASIC);
	ShaderManager::instance()->setUniformMatrix4fv(SHADER_MATERIAL_BASIC, "view", view);
	ShaderManager::instance()->setUniformMatrix4fv(SHADER_MATERIAL_BASIC, "projection", projection);
	ShaderManager::instance()->setUniform3fv(SHADER_MATERIAL_BASIC, "light_0_color", light_0_color);
	ShaderManager::instance()->setUniform3fv(SHADER_MATERIAL_BASIC, "light_0_pos", lightBulb0.getPos());
	ShaderManager::instance()->setUniform3fv(SHADER_MATERIAL_BASIC, "eyePos", camera.getPos());

	// draw
	for (int i = 0; i < total_gameObjects; ++i)
	{
		cubes[i].Draw(SHADER_MATERIAL_BASIC);
	}

	// use shader
	ShaderManager::instance()->useShader(SHADER_LIGHT_SOURCE);
	ShaderManager::instance()->setUniformMatrix4fv(SHADER_LIGHT_SOURCE, "view", view);
	ShaderManager::instance()->setUniformMatrix4fv(SHADER_LIGHT_SOURCE, "projection", projection);

	lightBulb0.Draw(SHADER_LIGHT_SOURCE);
}