#include "Engine.h"
// texture loading
#include <STB/stb_image.h>
Engine* Engine::engine = NULL;

void Engine::onWindowSizeUpdate(int width, int height)
{
	windowsWidth = width;
	windowsHeight = height;
	instance()->scene->windowsResize();
}

void Engine::onMouseUpdate(double xpos, double ypos)
{
	engine->scene->mouse_callback(xpos, ypos);
}

Engine::Engine()
{
	scene = NULL;
}

Engine::~Engine()
{
	delete TextureBuilder::instance();
	delete MeshBuilder::instance();
	delete MaterialBuilder::instance();
	delete ShaderManager::instance();
	delete scene;
}

Engine* Engine::instance()
{
	if (!engine)
	{
		engine = new Engine();
	}
	return engine;
}

void Engine::Init()
{
	// texture assets
	stbi_set_flip_vertically_on_load(true);
	TextureBuilder::instance()->Init();
	MeshBuilder::instance()->Init();
	MaterialBuilder::instance()->Init();
	ShaderManager::instance()->Init();

	scene = new SceneLightMap();
	scene->Init();
}

void Engine::Update(bool inputType[INPUT_TOTAL], float deltaTime)
{
	if (inputType[INPUT_ENABLE_WIREFRAME])
	{
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else if (inputType[INPUT_DISABLE_WIREFRAME])
	{
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	// update scene
	scene->Update(inputType, deltaTime);
}

void Engine::Draw()
{
	// rendering commands
	glClearColor(175.f / 255.f, 88.f / 255.f, 6.f / 255.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// render scene
	scene->Draw();
}