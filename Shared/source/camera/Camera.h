#pragma once
#include "../top/Utility.h"

class Camera
{
	glm::vec3 camPos, camTarget, camDir, camRightAxis, camUpAxis;
	float yaw, pitch;
	float lastX, lastY;
	bool firstMouse;

public:
	Camera();
	~Camera();

	void Init();
	void mouse_callback(double xpos, double ypos);
	void UpdateInput(bool inputList[INPUT_TOTAL], float deltaTime);
	void genLookUpMatrix(glm::mat4& lookat);
	glm::vec3 getPos();
};