#version 460 core
out vec4 FragColor;

struct Material {
	float ambient;
	float diffuse;
	float specular;
	float shininess;
};

in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

uniform float alphaValue;
uniform sampler2D textureSampler2D;
uniform vec3 light_0_color;
uniform vec3 light_0_pos;
uniform vec3 eyePos;
uniform Material material;

void main()
{
	// ambient
	vec3 ambient = material.ambient * light_0_color;
	
	// diffuse
	vec3 lightDir = normalize(light_0_pos - fragPos);
	// normalize the normals taken from vertex as it will be manipulated by vertex shader and magnitude no longer 1.0
	vec3 norm = normalize(normal);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = (diff * material.diffuse) * light_0_color;
	
	// specular
	vec3 viewDir = normalize(eyePos - fragPos);
	// reflect() gives a new vec from the light dir and normal dir (note it expects both input vecs to be pointing towards the frag)
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = material.specular * spec * light_0_color;
	
	FragColor = vec4(ambient + diffuse + specular, 1.0) * texture(textureSampler2D, texCoord) * alphaValue;
}