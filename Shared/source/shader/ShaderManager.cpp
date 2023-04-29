#include "ShaderManager.h"
ShaderManager* ShaderManager::shaderManager = NULL;

ShaderManager::ShaderManager()
{
}

ShaderManager::~ShaderManager()
{
}

ShaderManager* ShaderManager::instance()
{
    if (!shaderManager)
    {
        shaderManager = new ShaderManager();
    }
    return shaderManager;
}

void ShaderManager::Init()
{
    // create shaders
    shaders[SHADER_LIGHT_MAP].Init(shaderspath + "/lightMap/vertexshader.cpp", shaderspath + "/lightMap/fragmentshader.cpp");
    isUsed[SHADER_LIGHT_MAP] = false;
    shaders[SHADER_COLOR].Init(shaderspath + "/color/vertexshader.cpp", shaderspath + "/color/fragmentshader.cpp");
    isUsed[SHADER_COLOR] = false;

    LogInfo("ShaderManager initialized");
}

void ShaderManager::useShader(SHADER_TYPES type)
{
    if (!isUsed[type])
    {
        for (std::map<SHADER_TYPES, bool>::iterator it = isUsed.begin(); it != isUsed.end(); ++it)
        {
            (*it).second = false;
        }
        isUsed[type] = true;
        shaders[type].activateShader();
    }
}

void ShaderManager::setUniform1f(SHADER_TYPES type, std::string label, float value)
{
    useShader(type);
    shaders[type].setUniform1f(label, value);
}

void ShaderManager::setUniform1i(SHADER_TYPES type, std::string label, int value)
{
    useShader(type);
    shaders[type].setUniform1i(label, value);
}

void ShaderManager::setUniform3fv(SHADER_TYPES type, std::string label, glm::vec3 value)
{
    useShader(type);
    shaders[type].setUniform3fv(label, value);
}

void ShaderManager::setUniformMatrix4fv(SHADER_TYPES type, std::string label, glm::mat4& value)
{
    useShader(type);
    shaders[type].setUniformMatrix4fv(label, value);
}
