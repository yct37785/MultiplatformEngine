attribute vec3 aPos;
attribute vec3 aNormal;
attribute vec2 aTexCoord;

varying vec2 texCoord;
varying vec3 normal;
varying vec3 fragPos;	// for fragments pos in world space

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