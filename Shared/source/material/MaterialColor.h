#include "Material.h"

#pragma once
class MaterialColor : public Material
{
	glm::vec3 color;
public:
	MaterialColor();
	~MaterialColor();

	// please define your own Init
	void Init(SHADER_TYPES shaderType, glm::vec3 color);

	void LoadMaterial();
};

