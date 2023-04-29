#include "MeshBuilder.h"
MeshBuilder* MeshBuilder::meshBuilder = NULL;

MeshBuilder::MeshBuilder()
{
	for (int i = 0; i < MESH_TOTAL; ++i)
	{
		meshes[i] = NULL;
	}
}

MeshBuilder::~MeshBuilder()
{
	for (int i = 0; i < MESH_TOTAL; ++i)
	{
		delete meshes[i];
	}
}

MeshBuilder* MeshBuilder::instance()
{
	if (!meshBuilder)
	{
		meshBuilder = new MeshBuilder();
	}
	return meshBuilder;
}

void MeshBuilder::Init()
{
	initPrimitives();
    LogInfo("MeshBuilder initialized");
}

void MeshBuilder::initPrimitives()
{
    meshes[MESH_SPHERE] = genSphere();
    meshes[MESH_CUBE] = genCube();
    meshes[MESH_LINE] = genLine();
    meshes[MESH_QUAD_INFINITE_FLOOR] = genQuad(100);
}

Mesh* MeshBuilder::genCube()
{
    // cube
    /*
        top view:
               face 3
                ----
        face 2 |    | face 4
                ----
               face 1

        side view:
            face 6
            |    |
            face 5
    */
    std::vector<float> vertices {
        // position	         // normal        // tex coord
        // face 1
         0.5f,  0.5f,  0.5f,  0.f,  0.f,  1.f,  1.0f, 1.0f,	// 0
         0.5f, -0.5f,  0.5f,  0.f,  0.f,  1.f,  1.0f, 0.0f,	// 1
        -0.5f, -0.5f,  0.5f,  0.f,  0.f,  1.f,  0.0f, 0.0f,	// 2
        -0.5f,  0.5f,  0.5f,  0.f,  0.f,  1.f,  0.0f, 1.0f,	// 3
        // face 2
        -0.5f,  0.5f,  0.5f, -1.f,  0.f,  0.f,  1.0f, 1.0f,	// 4
        -0.5f, -0.5f,  0.5f, -1.f,  0.f,  0.f,  1.0f, 0.0f,	// 5
        -0.5f, -0.5f, -0.5f, -1.f,  0.f,  0.f,  0.0f, 0.0f,	// 6
        -0.5f,  0.5f, -0.5f, -1.f,  0.f,  0.f,  0.0f, 1.0f,	// 7
        // face 3
        -0.5f,  0.5f, -0.5f,  0.f,  0.f, -1.f,  1.0f, 1.0f,	// 8
        -0.5f, -0.5f, -0.5f,  0.f,  0.f, -1.f,  1.0f, 0.0f,	// 9
         0.5f, -0.5f, -0.5f,  0.f,  0.f, -1.f,  0.0f, 0.0f,	// 10
         0.5f,  0.5f, -0.5f,  0.f,  0.f, -1.f,  0.0f, 1.0f,	// 11
        // face 4
         0.5f,  0.5f, -0.5f,  1.f,  0.f,  0.f,  1.0f, 1.0f,	// 12
         0.5f, -0.5f, -0.5f,  1.f,  0.f,  0.f,  1.0f, 0.0f,	// 13
         0.5f, -0.5f,  0.5f,  1.f,  0.f,  0.f,  0.0f, 0.0f,	// 14
         0.5f,  0.5f,  0.5f,  1.f,  0.f,  0.f,  0.0f, 1.0f,	// 15
        // face 5
        -0.5f, -0.5f, -0.5f,  0.f, -1.f,  0.f,  0.0f, 1.0f,	// 19
        -0.5f, -0.5f,  0.5f,  0.f, -1.f,  0.f,  0.0f, 0.0f,	// 18
         0.5f, -0.5f,  0.5f,  0.f, -1.f,  0.f,  1.0f, 0.0f,	// 17
         0.5f, -0.5f, -0.5f,  0.f, -1.f,  0.f,  1.0f, 1.0f,	// 16
        // face 6
         0.5f,  0.5f, -0.5f,  0.f,  1.f,  0.f,  1.0f, 1.0f,	// 20
         0.5f,  0.5f,  0.5f,  0.f,  1.f,  0.f,  1.0f, 0.0f,	// 21
        -0.5f,  0.5f,  0.5f,  0.f,  1.f,  0.f,  0.0f, 0.0f,	// 22
        -0.5f,  0.5f, -0.5f,  0.f,  1.f,  0.f,  0.0f, 1.0f	// 23
    };

    // always draw our vertices in a CW direction
    std::vector<int> indices(6 * 6);
    for (int i = 0, j = 0; i < 6 * 6; i += 6, j += 4)
    {
        // triangle 1
        indices[i + 0] = j + 0;
        indices[i + 1] = j + 1;
        indices[i + 2] = j + 3;
        // triangle 2
        indices[i + 3] = j + 1;
        indices[i + 4] = j + 2;
        indices[i + 5] = j + 3;
    };

    return new Mesh(vertices, indices);
}

Mesh* MeshBuilder::genSphere()
{
    unsigned int VBO, EBO, VAO;

    std::vector<float> vertices; // x, y, z, nx, ny, nz, s, t
	float radius = 0.5f;
	int sectorCount = 36;
	int stackCount = 36;

	float x, y, z, xy;                              // vertex position
	float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
	float s, t;                                     // vertex texCoord

	float sectorStep = 2 * PI / sectorCount;
	float stackStep = PI / stackCount;
	float sectorAngle, stackAngle;

	for (int i = 0; i <= stackCount; ++i)
	{
		stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
		xy = radius * cosf(stackAngle);             // r * cos(u)
		z = radius * sinf(stackAngle);              // r * sin(u)

		// add (sectorCount+1) vertices per stack
		// the first and last vertices have same position and normal, but different tex coords
		for (int j = 0; j <= sectorCount; ++j)
		{
			sectorAngle = j * sectorStep;           // starting from 0 to 2pi

			// vertex position (x, y, z)
			x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
			y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);

			// normalized vertex normal (nx, ny, nz)
			nx = x * lengthInv;
			ny = y * lengthInv;
			nz = z * lengthInv;
            vertices.push_back(nx);
            vertices.push_back(ny);
            vertices.push_back(nz);

			// vertex tex coord (s, t) range between [0, 1]
			s = (float)j / sectorCount;
			t = (float)i / stackCount;
            vertices.push_back(s);
            vertices.push_back(t);
		}
	}

	// generate CW index list of sphere triangles
    // k1--k1+1
    // |  / |
    // | /  |
    // k2--k2+1
    std::vector<int> indices;
    std::vector<int> lineIndices;
	int k1, k2;
	for (int i = 0; i < stackCount; ++i)
	{
		k1 = i * (sectorCount + 1);     // beginning of current stack
		k2 = k1 + sectorCount + 1;      // beginning of next stack

		for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
		{
			// 2 triangles per sector excluding first and last stacks
			// k1 => k2 => k1+1
			if (i != 0)
			{
				indices.push_back(k1);
                indices.push_back(k1 + 1);
				indices.push_back(k2);
			}

			// k1+1 => k2 => k2+1
			if (i != (stackCount - 1))
			{
				indices.push_back(k1 + 1);
                indices.push_back(k2 + 1);
				indices.push_back(k2);
			}
		}
	}

    return new Mesh(vertices, indices);
}

Mesh* MeshBuilder::genLine()
{
    // we can disregard the normals and texcoordsS
    std::vector<float> vertices {
        -0.5f, 0.0f, 0.0f, 0.f, 0.f, 1.f, 1.0f, 1.0f,
        0.5f, 0.0f, 0.0f, 0.f, 0.f, 1.f, 1.0f, 0.0f
    };

    // always draw our vertices in a CW direction
    std::vector<int> indices { 0, 1 };

    return new Mesh(vertices, indices);
}

Mesh* MeshBuilder::genQuad(int scale)
{
    float unit = (float)scale;
    std::vector<float> vertices {
         unit * 0.5f,  0.f,  unit * 0.5f,  0.f,  1.f,  0.f,  unit, unit,
         unit * 0.5f,  0.f, -unit * 0.5f,  0.f,  1.f,  0.f,  unit, 0.0f,
        -unit * 0.5f,  0.f, -unit * 0.5f,  0.f,  1.f,  0.f,  0.0f, 0.0f,
        -unit * 0.5f,  0.f,  unit * 0.5f,  0.f,  1.f,  0.f,  0.0f, unit
    };

    // always draw our vertices in a CW direction
    std::vector<int> indices {
        0, 3, 1, 3, 2, 1
    };

    return new Mesh(vertices, indices);
}

const Mesh* MeshBuilder::getMesh(MESH_TYPES type)
{
	if (type != MESH_TOTAL)
	{
		return meshes[type];
	}
	else
	{
		return NULL;
	}
}