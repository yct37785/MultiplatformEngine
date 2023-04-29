#pragma once
#include "Shader.h"

class ShaderManager
{
	static ShaderManager* shaderManager;
	Shader shaders[SHADER_TOTAL];
	std::map< SHADER_TYPES, bool> isUsed;

	ShaderManager();

public:
	~ShaderManager();
	static ShaderManager* instance();

	void Init();
	void useShader(SHADER_TYPES type);

	void setUniform1f(SHADER_TYPES type, std::string label, float value);
	void setUniform1i(SHADER_TYPES type, std::string label, int value);
	void setUniform3fv(SHADER_TYPES type, std::string label, glm::vec3 value);
	void setUniformMatrix4fv(SHADER_TYPES type, std::string label, glm::mat4& value);
};

