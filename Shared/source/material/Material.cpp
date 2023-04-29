#include "Material.h"

Material::Material()
{
}

Material::~Material()
{
}

void Material::Init(SHADER_TYPES shaderType)
{
	this->shaderType = shaderType;
}