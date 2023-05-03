#include "SceneLightMap.h"

SceneLightMap::SceneLightMap()
{
}

SceneLightMap::~SceneLightMap()
{
}

void SceneLightMap::Init()
{
	// any flags please set here

	InitObjects();
	camera.Init();

	// proj
	projection = glm::perspective(glm::radians(45.f), (float)windowsWidth / (float)windowsHeight, 0.1f, 100.f);

	// activate to set uniforms
	ShaderManager::instance()->useShader(SHADER_LIGHT_MAP);

	LogInfo("Scene initialized");
}

void SceneLightMap::InitObjects()
{
	// objects
	cubes[0].Init(MESH_CUBE, MAT_WOODEN_CRATE, glm::vec3(1.f, 1.f, 1.f), glm::vec3(2.f, 0.4f, 0.f), 5.f, -10.f);
	cubes[1].Init(MESH_CUBE, MAT_WOODEN_CRATE, glm::vec3(1.f, 1.f, 1.f), glm::vec3(-2.f, 0.f, 0.f), 0.f, 0.f);
	cubes[2].Init(MESH_CUBE, MAT_WOODEN_CRATE, glm::vec3(1.f, 1.f, 1.f), glm::vec3(0.f, 2.f, 0.5f), 50.f, 20.f);
	cubes[3].Init(MESH_SPHERE, MAT_WOODEN_CRATE, glm::vec3(1.5f, 1.5f, 1.5f), glm::vec3(0.f, 0.7f, -2.f), 45.f, 0.f);
	cubes[4].Init(MESH_CUBE, MAT_WOODEN_CRATE, glm::vec3(2.f, 2.f, 2.f), glm::vec3(0.f, 5.f, 5.f), 20.f, 160.f);

	// light
	lightBulb0.Init(MESH_SPHERE, MAT_WHITE_COLOR, glm::vec3(0.15f, 0.15f, 0.15f), glm::vec3(0.f, 0.f, 0.0f), 0.f, 0.f);
	light_0_color = glm::vec3(1.f, 1.f, 1.f);
	lightAmbient = 0.2f;
	lightDiffuse = 0.5f;
	lightSpecular = 1.f;
}

void SceneLightMap::Update(bool inputList[INPUT_TOTAL], float deltaTime)
{
	// test some changes
	// last
	camera.UpdateInput(inputList, deltaTime);
	UpdateTransformation(deltaTime);
}

void SceneLightMap::mouse_callback(double xpos, double ypos)
{
	camera.mouse_callback(xpos, ypos);
}

void SceneLightMap::UpdateTransformation(float deltaTime)
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
}

void SceneLightMap::UpdateViewMatrix()
{
	// lookat matrix
	// we create a matrix with the above 3 perpendicular axes + translation vector
	// this matrix can transform any vector to the view coord space just by multiplying with it
	// more details: https://learnopengl.com/Getting-started/Camera
	camera.genLookUpMatrix(view);
}

void SceneLightMap::Draw()
{
	// use shader
	ShaderManager::instance()->useShader(SHADER_LIGHT_MAP);
	// mvp
	ShaderManager::instance()->setUniformMatrix4fv(SHADER_LIGHT_MAP, "view", view);
	ShaderManager::instance()->setUniformMatrix4fv(SHADER_LIGHT_MAP, "projection", projection);
	// light
	ShaderManager::instance()->setUniform3fv(SHADER_LIGHT_MAP, "light.position", lightBulb0.getPos());
	ShaderManager::instance()->setUniform3fv(SHADER_LIGHT_MAP, "light.color", light_0_color);
	ShaderManager::instance()->setUniform3fv(SHADER_LIGHT_MAP, "light.values", glm::vec3(lightAmbient, lightDiffuse, lightSpecular));
	// eye
	ShaderManager::instance()->setUniform3fv(SHADER_LIGHT_MAP, "eyePos", camera.getPos());
	// draw
	for (int i = 0; i < total_gameObjects; ++i)
	{
		cubes[i].Draw(SHADER_LIGHT_MAP);
	}
	DrawFloor(-0.5f, MAT_FLOOR_TILES);
	// use shader
	ShaderManager::instance()->useShader(SHADER_COLOR);
	ShaderManager::instance()->setUniformMatrix4fv(SHADER_COLOR, "view", view);
	ShaderManager::instance()->setUniformMatrix4fv(SHADER_COLOR, "projection", projection);
	lightBulb0.Draw(SHADER_COLOR);
	glLineWidth(2.f);
	DrawAxes(0.f, 0.f, MAT_GREEN_COLOR);
	DrawAxes(0.f, 90.f, MAT_BLUE_COLOR);
	DrawAxes(90.f, 0.f, MAT_RED_COLOR);
	glLineWidth(1.f);
}

void SceneLightMap::DrawFloor(float floorY, MAT_TYPES matType)
{
	glm::mat4 transformMat = glm::mat4(1.f);
	transformMat = glm::translate(transformMat, glm::vec3(0.f, floorY, 0.f));
	glm::mat4 normalMat = glm::transpose(glm::inverse(transformMat));

	glBindVertexArray(MeshBuilder::instance()->getMesh(MESH_QUAD_INFINITE_FLOOR)->getVAO());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, MeshBuilder::instance()->getMesh(MESH_QUAD_INFINITE_FLOOR)->getEBO());;

	ShaderManager::instance()->setUniformMatrix4fv(SHADER_LIGHT_MAP, "model", transformMat);
	ShaderManager::instance()->setUniformMatrix4fv(SHADER_LIGHT_MAP, "normalMat", normalMat);
	MaterialBuilder::instance()->LoadMaterial(matType);

	glDrawElements(GL_TRIANGLES, MeshBuilder::instance()->getMesh(MESH_QUAD_INFINITE_FLOOR)->getTotalVertices(), GL_UNSIGNED_INT, 0);
}

void SceneLightMap::DrawAxes(float pitch, float yaw, MAT_TYPES colorMat)
{
	glm::vec3 axesScale(10000.0, 1.0, 1.0);
	glm::mat4 transformMat = glm::mat4(1.f);
	transformMat = glm::rotate(transformMat, glm::radians(yaw), glm::vec3(0.f, 1.f, 0.f));
	transformMat = glm::rotate(transformMat, glm::radians(pitch), glm::vec3(0.f, 0.f, 1.f));
	transformMat = glm::scale(transformMat, axesScale);

	glBindVertexArray(MeshBuilder::instance()->getMesh(MESH_LINE)->getVAO());

	ShaderManager::instance()->setUniformMatrix4fv(SHADER_COLOR, "model", transformMat);
	MaterialBuilder::instance()->LoadMaterial(colorMat);

	glDrawElements(GL_LINES, MeshBuilder::instance()->getMesh(MESH_LINE)->getTotalVertices(), GL_UNSIGNED_INT, 0);
}

void SceneLightMap::windowsResize()
{
	// proj
	projection = glm::perspective(glm::radians(45.f), (float)windowsWidth / (float)windowsHeight, 0.1f, 100.f);
}