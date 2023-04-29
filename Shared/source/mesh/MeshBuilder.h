#pragma once
#include "Mesh.h"

// with thanks to: http://www.songho.ca/opengl/gl_sphere.html
class MeshBuilder
{
	static MeshBuilder* meshBuilder;
	Mesh* meshes[MESH_TOTAL];

	MeshBuilder();

	// generators
	Mesh* genCube();
	Mesh* genSphere();
	Mesh* genLine();
	Mesh* genQuad(int scale);

	// primitives
	void initPrimitives();

public:
	~MeshBuilder();
	static MeshBuilder* instance();

	void Init();
	const Mesh* getMesh(MESH_TYPES type);
};