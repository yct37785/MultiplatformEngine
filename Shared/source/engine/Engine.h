#pragma once
//#include "../scenes/SceneBasicTriangle.h"
//#include "../scenes/SceneBasicRectangle.h"
//#include "../scenes/SceneBasicShaders.h"
//#include "../scenes/SceneBasicTexture.h"
//#include "../scenes/SceneBasicTransform.h"
//#include "../scenes/SceneBasic3D.h"
//#include "../scenes/SceneBasicCamera.h"
// #include "../scenes/SceneBasicLighting.h"
// #include "../scenes/SceneBasicMaterial.h"
#include "../scenes/SceneLightMap.h"

class Engine
{
	static Engine* engine;
	Scene* scene;

private:
	Engine();

public:
	~Engine();
	static Engine* instance();

	// init
	void Init();

	// update
	void Update(bool inputList[INPUT_TOTAL], float deltaTime);

	// event handling
	void onWindowSizeUpdate(int width, int height);
	void onMouseUpdate(double xpos, double ypos);

	// draw
	void Draw();
};