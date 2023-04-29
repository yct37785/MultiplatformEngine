#pragma once
#include "../top/Utility.h"
#include "../camera/Camera.h"
#include "../object/GameObject.h"

/*
* Used for quick and easy implementations for experimentations
*/
class Scene
{
public:
	Scene();
	virtual ~Scene();

	virtual void Init() = 0;
	virtual void Update(bool inputList[INPUT_TOTAL], float deltaTime) = 0;
	virtual void Draw() = 0;

	// utilities
	virtual void windowsResize() = 0;
	virtual void mouse_callback(double xpos, double ypos);
};