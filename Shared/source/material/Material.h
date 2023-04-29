#pragma once
#include "../shader/ShaderManager.h"
#include "../texture/TextureBuilder.h"

/*
* properties (diff. map, spec. map etc) to be contained in children classes
* each material can only be used on a specified shader
*/
class Material
{
protected:
	SHADER_TYPES shaderType;

public:
	Material();
	virtual ~Material();

	// please define your own Init
	void Init(SHADER_TYPES shaderType);

	virtual void LoadMaterial() = 0;
};