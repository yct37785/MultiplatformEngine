#version 460 core
out vec4 FragColor;

in vec3 vertexColor; // the input variable from the vertex shader (same name and same type)

uniform float alphaValue;

void main()
{
    FragColor = vec4(vertexColor, alphaValue);
}