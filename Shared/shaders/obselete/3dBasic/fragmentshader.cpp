#version 460 core
out vec4 FragColor;

in vec3 vertexColor;
in vec2 texCoord;

uniform float alphaValue;
uniform sampler2D textureSampler2D;
uniform sampler2D textureSampler2D_1;

void main()
{
	// FragColor = vec4(vertexColor, 1.0);
	// just texture
	// FragColor = texture(textureSampler2D, texCoord);
	// mix with vertex color
	// FragColor = texture(textureSampler2D, texCoord) * vec4(vertexColor, 1.0);
	// mix with 2 textures
	FragColor = mix(texture(textureSampler2D, texCoord), texture(textureSampler2D_1, texCoord), 0.5) * alphaValue;
}