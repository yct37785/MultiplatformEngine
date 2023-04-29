#include "Camera.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::Init()
{
	camPos = glm::vec3(0.f, 0.f, 3.f);	// backwards by 3.f
	// take note: if you want to change pos/target make sure to change mouse_callback magnitude accordingly
	camTarget = glm::vec3(0.f, 0.f, 2.f);	// point our camera is looking at
	pitch = 0.f;
	yaw = 90.f;
	lastX = 400.f;
	lastY = 300.f;
	firstMouse = true;
}

void Camera::mouse_callback(double xpos, double ypos)
{
	// mouse input
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = ypos - lastY;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (yaw >= 360.f) { yaw -= 360.f; }
	else if (yaw < 0.f) { yaw += 360.f; }
	if (pitch > 89.0f) { pitch = 89.0f; }
	if (pitch < -89.0f) { pitch = -89.0f; }

	// derive the target
	// rmb: toa cah soh
	// in a triangle, we have hypotenuse h.
	// cos(yaw) = x length / h = rate of X axis in regards to dir
	// sin(yaw) = z length / h = rate of Z axis in regards to dir
	// sin(pitch) = rate of Y axis in regards to dir

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	camTarget = camPos - direction;
}

void Camera::UpdateInput(bool inputList[INPUT_TOTAL], float deltaTime)
{
	// we already defined the pos and target
	camDir = glm::normalize(camTarget - camPos);	// unit vector of the dir

	// camera right axis
	camUpAxis = glm::vec3(0.f, 1.f, 0.f);	// first we set the up axis to absolutely point upwards
	// the up axis X dir = a perpendicular to both direction
	// the angle in btw doesn't matter
	// because of OpenGL's right hand rule, we want to get the right axis with the opp. of the dir
	camRightAxis = glm::normalize(glm::cross(camUpAxis, -camDir));

	// with the right axis we can get the camera's local up axis
	camUpAxis = glm::cross(camDir, -camRightAxis);	// both are already normalized

	// manipulate transformations etc
	const float camSpeed = 2.5f * deltaTime; // adjust accordingly
	if (inputList[INPUT_FWD])
	{
		camPos += camSpeed * camDir;
		camTarget += camSpeed * camDir;
	}
	if (inputList[INPUT_BWD])
	{
		camPos -= camSpeed * camDir;
		camTarget -= camSpeed * camDir;
	}
	if (inputList[INPUT_LEFT])
	{
		camPos -= camSpeed * camRightAxis;
		camTarget -= camSpeed * camRightAxis;
	}
	if (inputList[INPUT_RIGHT])
	{
		camPos += camSpeed * camRightAxis;
		camTarget += camSpeed * camRightAxis;
	}
	if (inputList[INPUT_DW])
	{
		glm::vec3 absUp(0.f, 1.f, 0.f);
		camPos -= camSpeed * absUp;
		camTarget -= camSpeed * absUp;
	}
	if (inputList[INPUT_UP])
	{
		glm::vec3 absUp(0.f, 1.f, 0.f);
		camPos += camSpeed * absUp;
		camTarget += camSpeed * absUp;
	}
}

void Camera::genLookUpMatrix(glm::mat4& lookat)
{
	lookat = glm::lookAt(camPos, camTarget, camUpAxis);
}

glm::vec3 Camera::getPos()
{
	return camPos;
}