#pragma once
#include "../top/Utility.h"

class Shader
{
	unsigned int shaderID;

public:
	Shader();
	~Shader();

	void Init(std::string vertexShaderSource, std::string fragmentShaderSource);
	void activateShader();

	void setUniform1f(std::string label, float value);
	void setUniform1i(std::string label, int value);
	void setUniform3fv(std::string label, glm::vec3 value);
	void setUniformMatrix4fv(std::string label, glm::mat4& value);
};

