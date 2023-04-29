#include "MaterialLightMap.h"

void MaterialLightMap::Init(SHADER_TYPES shaderType, TEX_TYPES diffuseMap, TEX_TYPES specularMap, float shininess)
{
	Material::Init(shaderType);
	this->diffuseMap = diffuseMap;
	this->specularMap = specularMap;
	this->shininess = shininess;
}

void MaterialLightMap::LoadMaterial()
{
	// bind texture
	glActiveTexture(GL_TEXTURE0); // activate the texture unit first (sampler2D in fragment shader)
	glBindTexture(GL_TEXTURE_2D, TextureBuilder::instance()->getTexture(diffuseMap));
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, TextureBuilder::instance()->getTexture(specularMap));

	// set uniforms
	ShaderManager::instance()->setUniform1i(shaderType, "material.diffuse", 0);	// diffuse is GL_TEXTURE0
	ShaderManager::instance()->setUniform1i(shaderType, "material.specular", 1);	// specular is GL_TEXTURE1
	ShaderManager::instance()->setUniform1f(shaderType, "material_shininess", shininess);
}
