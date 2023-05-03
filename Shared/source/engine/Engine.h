#pragma once
#include <ProjectSelector.h>

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