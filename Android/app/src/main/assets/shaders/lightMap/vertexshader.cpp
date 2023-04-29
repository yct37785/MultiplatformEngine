#version 300 es
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 texCoord;
out vec3 normal;
out vec3 fragPos;	// for fragments pos in world space

uniform mat4 normalMat;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	// world space
	vec4 worldSpace = model * vec4(aPos, 1.0);
	// view and proj space
    gl_Position = projection * view * worldSpace;
	texCoord = aTexCoord;
	normal = vec3(normalMat * vec4(aNormal, 0.0));	// leave w to 0.0 as we don't want translation applied
	fragPos = worldSpace.xyz;
}