#version 460 core
out vec4 FragColor;

struct Material {
	sampler2D diffuse;
    sampler2D specular;
};

struct Light {
	vec3 position;
	vec3 color;
	vec3 values;	// 0: ambient, 1: diffuse, 2: specular (save uniform calls by using vec3)
};

in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

// light
uniform Light light;
// material
uniform Material material;
uniform float material_shininess;
// others
uniform vec3 eyePos;

void main()
{
	// vars
	vec3 norm = normalize(normal);
	vec3 viewDir = normalize(eyePos - fragPos);
	vec3 lightDir = normalize(light.position - fragPos);
	
	// light colors
	vec3 lightAmbient = light.values.x * light.color;
	vec3 lightDiffuse = light.values.y * light.color;
	vec3 lightSpecular = light.values.z * light.color;
	
	// ambient
	vec3 diffuseTex = vec3(texture(material.diffuse, texCoord));
	vec3 ambient = lightAmbient * diffuseTex;
	
	// diffuse
	// normalize the normals taken from vertex as it will be manipulated by vertex shader and magnitude no longer 1.0
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = lightDiffuse * diff * diffuseTex;
	
	// specular
	// reflect() gives a new vec from the light dir and normal dir (note it expects both input vecs to be pointing away from frag)
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material_shininess);
	vec3 specular = lightSpecular * spec * vec3(texture(material.specular, texCoord));
	
	// final
	FragColor = vec4(ambient + diffuse + specular, 1.0);
}