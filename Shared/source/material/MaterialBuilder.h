#pragma once
#include "MaterialLightMap.h"
#include "MaterialColor.h"

class MaterialBuilder
{
	static MaterialBuilder* materialBuilder;
	Material* materials[MAT_TOTAL];

	MaterialBuilder();

public:
	~MaterialBuilder();
	static MaterialBuilder* instance();

	void Init();
	void LoadMaterial(MAT_TYPES type);
};