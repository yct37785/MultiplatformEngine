#include "MaterialBuilder.h"
MaterialBuilder* MaterialBuilder::materialBuilder = NULL;

MaterialBuilder::MaterialBuilder()
{
	for (int i = 0; i < MAT_TOTAL; ++i)
	{
		materials[i] = NULL;
	}
}

MaterialBuilder::~MaterialBuilder()
{
	for (int i = 0; i < MAT_TOTAL; ++i)
	{
		delete materials[i];
	}
}

MaterialBuilder* MaterialBuilder::instance()
{
	if (!materialBuilder)
	{
		materialBuilder = new MaterialBuilder();
	}
    return materialBuilder;
}

void MaterialBuilder::Init()
{
	MaterialColor* matColor = new MaterialColor();
	matColor->Init(SHADER_COLOR, glm::vec3(1.f, 1.f, 1.f));
	materials[MAT_WHITE_COLOR] = matColor;
	matColor = new MaterialColor();
	matColor->Init(SHADER_COLOR, glm::vec3(1.f, 0.f, 0.f));
	materials[MAT_RED_COLOR] = matColor;
	matColor = new MaterialColor();
	matColor->Init(SHADER_COLOR, glm::vec3(0.f, 1.f, 0.f));
	materials[MAT_GREEN_COLOR] = matColor;
	matColor = new MaterialColor();
	matColor->Init(SHADER_COLOR, glm::vec3(0.f, 0.f, 1.f));
	materials[MAT_BLUE_COLOR] = matColor;

	MaterialLightMap* matLightMap = new MaterialLightMap();
	matLightMap->Init(SHADER_LIGHT_MAP, TEX_WOODEN_CRATE_FACE, TEX_WOODEN_CRATE_FACE_SPECULAR, 128);
	materials[MAT_WOODEN_CRATE] = matLightMap;
	matLightMap = new MaterialLightMap();
	matLightMap->Init(SHADER_LIGHT_MAP, TEX_FLOOR_TILES, TEX_FLOOR_TILES_SPECULAR, 170);
	materials[MAT_FLOOR_TILES] = matLightMap;

	LogInfo("MaterialBuilder initialized");
}

void MaterialBuilder::LoadMaterial(MAT_TYPES type)
{
	if (type != MAT_TOTAL && materials[type])
	{
		materials[type]->LoadMaterial();
	}
}