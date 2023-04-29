#pragma once
#include "../top/Utility.h"

/*
* VBO, EBO and VAO
* vertices, texcoords, normals etc
* attach textures separately
*/
class Mesh
{
	unsigned int VBO, EBO, VAO;
	std::vector<float> vertices;
	std::vector<int> indices;

public:
	Mesh(const std::vector<float>& vertices, const std::vector<int>& indices);
	~Mesh();

	unsigned int getVBO() const;
	unsigned int getEBO() const;
	unsigned int getVAO() const;
	int getTotalVertices() const;
};