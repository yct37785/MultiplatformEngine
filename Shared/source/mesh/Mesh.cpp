#include "Mesh.h"

Mesh::Mesh(const std::vector<float>& vertices, const std::vector<int>& indices)
{
    this->vertices = vertices;
    this->indices = indices;
    // VAO (stores all attributes of the following GL_ARRAY_BUFFER)
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    // EBO
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);
    // VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);	// bind the above created buffer object to this target
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(int), vertices.data(), GL_STATIC_DRAW);	// copy user defined data to the bounded buffer
    // position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    // normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    // tex attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    // unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

unsigned int Mesh::getVBO() const
{
	return VBO;
}

unsigned int Mesh::getEBO() const
{
	return EBO;
}

unsigned int Mesh::getVAO() const
{
	return VAO;
}

int Mesh::getTotalVertices() const
{
	return vertices.size();
}
