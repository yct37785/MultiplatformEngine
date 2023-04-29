#pragma once
#include "../top/Utility.h"

class TextureBuilder
{
	static TextureBuilder* textureBuilder;
	unsigned int textures[TEX_TOTAL];

	TextureBuilder();

public:
	~TextureBuilder();
	static TextureBuilder* instance();

	void Init();
	unsigned int getTexture(TEX_TYPES type);
};

