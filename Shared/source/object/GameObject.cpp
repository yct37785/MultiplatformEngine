#include "GameObject.h"

GameObject::GameObject()
{
	transformMat = glm::mat4(1.f);
}

GameObject::~GameObject()
{
}

void GameObject::Init(MESH_TYPES meshType, MAT_TYPES matType, glm::vec3 scale, glm::vec3 pos, float yaw, float pitch)
{
	this->meshType = meshType;
	this->matType = matType;
	this->scale = scale;
	this->pos = pos;

	this->pitch = pitch;
	this->yaw = yaw;

	// derive dir
	dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	dir.y = sin(glm::radians(pitch));
	dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	// derive up, right
	// same logic as camera
	up = glm::vec3(0.f, 1.f, 0.f);
	right = glm::normalize(glm::cross(up, -dir));
	up = glm::cross(dir, -right);
}

void GameObject::UpdateTransform()
{
	transformMat = glm::mat4(1.f);
	transformMat = glm::translate(transformMat, pos);
	transformMat = glm::rotate(transformMat, glm::radians(yaw), up);
	transformMat = glm::rotate(transformMat, glm::radians(pitch), right);
	transformMat = glm::scale(transformMat, scale);

	// in short:
	// transpose matrix: "reverses a mat, etc. reverse a transformation"
	// inverse matrix: "1 / matrix", mat * inverseMat = identity mat
	// normalMat purpose is to make sure all transformations applied to it's vertices are reflected on the normals too
	// transformMat itself is sufficient for applying transformations to the normals, but non-uniform scale will skew
	// the vertices and the normals won't be accurate anymore
	// getting the inverse does the trick (more reading required), then we transpose the matrix to "reverse it back"
	normalMat = glm::transpose(glm::inverse(transformMat));
}

void GameObject::Draw(SHADER_TYPES shaderType)
{
	// bind VAO
	glBindVertexArray(MeshBuilder::instance()->getMesh(meshType)->getVAO());

	// uniforms (transformation)
	ShaderManager::instance()->setUniformMatrix4fv(shaderType, "model", transformMat);
	ShaderManager::instance()->setUniformMatrix4fv(shaderType, "normalMat", normalMat);

	// uniforms (material)
	MaterialBuilder::instance()->LoadMaterial(matType);

	// draws the primitves with indices (stored in EBO)
	glDrawElements(GL_TRIANGLES, MeshBuilder::instance()->getMesh(meshType)->getTotalVertices(), GL_UNSIGNED_INT, 0);
}

const glm::mat4& GameObject::getTransformMat()
{
	return transformMat;
}

const glm::vec3 GameObject::getPos()
{
	return pos;
}

void GameObject::rotate(float yaw, float pitch)
{
	this->pitch += pitch;
	this->yaw += yaw;
	if (yaw >= 360.f) { yaw -= 360.f; }
	else if (yaw < 0.f) { yaw += 360.f; }

	// derive dir
	dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	dir.y = sin(glm::radians(pitch));
	dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	// derive up, right
	up = glm::vec3(0.f, 1.f, 0.f);
	right = glm::normalize(glm::cross(up, -dir));
	up = glm::cross(dir, -right);
}