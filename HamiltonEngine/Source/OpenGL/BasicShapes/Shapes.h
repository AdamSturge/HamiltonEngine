#pragma once

namespace HamiltonEngine::OpenGL
{
	const float UNIT_TETRA_VERTS[] =
	{
        0.0f, 0.0f, 0.0f,
        -2.0f, 0.0f, -2.0f,
        0.0f, -2.0f, -2.0f,
        -2.0f, -2.0f, 0.0f
	};

    const float UNIT_TETRA_TEXTURE_COORD[] =
    {
        // these should be 1:1 with the vertices
        0.0f, 0.0f,
        -1.0f, -1.0f,
        1.0f, -1.0f,
        -1.0f, 1.0f
    };

    const unsigned int TETRA_VERT_INDICIES[] =
    {
        0, 1, 2, // tri1
        0, 1, 3,  // tri2
        0, 2, 3,  // tri3
        1, 2, 3,  // tri4

    };

    void CreateTetra(GLuint* VAO, GLuint* VBO);
    entt::entity CreateTetra(TransformComponent trans);
    entt::entity CreateSphere(TransformComponent trans, int Radius, int StackCount, int SectorCount);


    void TestTetras(int num);
}