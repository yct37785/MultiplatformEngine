#include "MaterialColor.h"

MaterialColor::MaterialColor()
{
}

MaterialColor::~MaterialColor()
{
}

void MaterialColor::Init(SHADER_TYPES shaderType, glm::vec3 color)
{
	Material::Init(shaderType);
	this->color = color;
}

void MaterialColor::LoadMaterial()
{
	// set uniforms
	ShaderManager::instance()->setUniform3fv(shaderType, "color", color);
}