#pragma once
#include "Material.h"

/*
* contains a diffuse map (the texture itself) and a specular map (another texture)
*/
class MaterialLightMap : public Material
{
	TEX_TYPES diffuseMap;
	TEX_TYPES specularMap;
	float shininess;

public:
	void Init(SHADER_TYPES shaderType, TEX_TYPES diffuseMap, TEX_TYPES specularMap, float shininess);
	void LoadMaterial();
};