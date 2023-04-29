#version 460 core
out vec4 FragColor;

in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

uniform float alphaValue;
uniform sampler2D textureSampler2D;
uniform vec3 light_0_color;
uniform vec3 light_0_pos;
uniform vec3 eyePos;

void main()
{
	// ambient
	float ambientStrength = 0.2;
	vec3 ambient = ambientStrength * light_0_color;
	
	// diffuse
	vec3 lightDir = normalize(light_0_pos - fragPos);
	// normalize the normals taken from vertex as it will be manipulated by vertex shader and magnitude no longer 1.0
	vec3 norm = normalize(normal);
	vec3 diffuse = max(dot(norm, lightDir), 0.0) * light_0_color;
	
	// specular
	float specularStrength = 0.5;
	vec3 viewDir = normalize(eyePos - fragPos);
	// reflect() gives a new vec from the light dir and normal dir (note it expects both input vecs to be pointing towards the frag)
	vec3 reflectDir = reflect(-lightDir, norm);
	float shininessValue = 128;
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininessValue);
	vec3 specular = specularStrength * spec * light_0_color;
	
	FragColor = vec4(ambient + diffuse + specular, 1.0) * texture(textureSampler2D, texCoord) * alphaValue;
}