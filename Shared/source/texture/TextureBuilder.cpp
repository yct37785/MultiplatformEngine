#include "TextureBuilder.h"
TextureBuilder* TextureBuilder::textureBuilder = NULL;

TextureBuilder::TextureBuilder()
{
	for (int i = 0; i < TEX_TOTAL; ++i)
	{
		textures[i] = 0;
	}
}

TextureBuilder::~TextureBuilder()
{
	glDeleteTextures(TEX_TOTAL, textures);
}

TextureBuilder* TextureBuilder::instance()
{
	if (!textureBuilder)
	{
		textureBuilder = new TextureBuilder();
	}
	return textureBuilder;
}

void TextureBuilder::Init()
{
	unsigned int texture = 0;
	generateTexture(assetsPath + "/textures/tile-floor.png", texture);
	textures[TEX_FLOOR_TILES] = texture;
	generateTexture(assetsPath + "/textures/tile-floor-specular.png", texture);
	textures[TEX_FLOOR_TILES_SPECULAR] = texture;
	generateTexture(assetsPath + "/textures/wooden-crate-face.png", texture);
	textures[TEX_WOODEN_CRATE_FACE] = texture;
	generateTexture(assetsPath + "/textures/wooden-crate-face-specular.png", texture);
	textures[TEX_WOODEN_CRATE_FACE_SPECULAR] = texture;

	LogInfo("TextureBuilder initialized");
}

unsigned int TextureBuilder::getTexture(TEX_TYPES type)
{
	if (type != TEX_TOTAL)
	{
		return textures[type];
	}
	else
	{
		return 0;
	}
}