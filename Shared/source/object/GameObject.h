#pragma once
#include "../mesh/MeshBuilder.h"
#include "../material/MaterialBuilder.h"

/*
* Very basic object with pos, scale, dir, up, right, yaw, pitch, mesh and a texture
* additional textures, normal maps etc. should be defined in children classes
*/
class GameObject
{
	MESH_TYPES meshType;
	MAT_TYPES matType;
	glm::vec3 pos, scale;
	glm::vec3 dir, up, right;	// all are unit vectors
	float yaw, pitch;
	glm::mat4 transformMat;
	glm::mat4 normalMat;

public:
	GameObject();
	~GameObject();

	virtual void Init(MESH_TYPES meshType, MAT_TYPES matType, glm::vec3 scale, glm::vec3 pos, float yaw, float pitch);

	// update
	virtual void UpdateTransform();
	
	// draw
	virtual void Draw(SHADER_TYPES shaderType);

	// get
	const glm::mat4& getTransformMat();
	const glm::vec3 getPos();

	// transform
	void rotate(float yaw, float pitch);
};