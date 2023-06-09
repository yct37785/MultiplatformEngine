#pragma once
#include <scenes/Scene.h>

/*
* Sample scene 3D
*/
class SceneTemplate : public Scene
{
	// an object
	const static int total_gameObjects = 5;
	GameObject cubes[total_gameObjects];

	// light
	GameObject lightBulb0;
	glm::vec3 light_0_color;
	float lightAmbient;
	float lightDiffuse;
	float lightSpecular;

	// camera
	Camera camera;

	// transformation
	glm::mat4 view, projection;

	void InitObjects();
	void UpdateTransformation(float deltaTime);
	void UpdateViewMatrix();

	void DrawFloor(float floorY, MAT_TYPES matType);
	void DrawAxes(float pitch, float yaw, MAT_TYPES colorMat);

public:
	SceneTemplate();
	~SceneTemplate();

	void Init();
	void Update(bool inputList[INPUT_TOTAL], float deltaTime);
	void Draw();
	
	void windowsResize();
	void mouse_callback(double xpos, double ypos);
};