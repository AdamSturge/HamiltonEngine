#include <PrecompiledHeader/Pch.h>
#include <Configuration/Globals.h>
#include <OpenGL/OpenGL.h>
#include <OpenGL/Utils.h>
#include "Tetra.h"
#include <Eigen/Dense>

namespace HamiltonEngine::OpenGL
{
	OpenGLBuffersComponent CreateTetraBuffers()
	{
		OpenGLBuffersComponent buffs = CreateOpenGLBuffersComponent(true);

		glBindVertexArray(buffs.VAO);
		glBindBuffer(GL_ARRAY_BUFFER, buffs.VBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(UNIT_TETRA_VERTS) + sizeof(UNIT_TETRA_TEXTURE_COORD), NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(UNIT_TETRA_VERTS), &UNIT_TETRA_VERTS);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(UNIT_TETRA_VERTS), sizeof(UNIT_TETRA_TEXTURE_COORD), &UNIT_TETRA_TEXTURE_COORD);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffs.EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(TETRA_VERT_INDICIES), TETRA_VERT_INDICIES, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(sizeof(UNIT_TETRA_VERTS)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		buffs.DrawMode = GL_TRIANGLES;
		buffs.start = 0;
		buffs.count = sizeof(TETRA_VERT_INDICIES) / sizeof(TETRA_VERT_INDICIES[0]); // Counting the number of vertexes that are being used.

		return buffs;
	}


	entt::entity CreateTetraEntity(TransformComponent trans)
	{
		entt::registry& reg = HamiltonEngine::Globals::Registry;
		const auto ent = reg.create();


		OpenGLBuffersComponent buffs = CreateTetraBuffers();

		reg.emplace<OpenGLBuffersComponent>(ent, buffs);
		reg.emplace<TransformComponent>(ent, trans);

		return ent;
	}

	void TestTetras(int num)
	{

		for (int i = 0; i < num; i++)
		{
			TransformComponent Trans;
			Trans = RandomTransformComponent();

			CreateTetraEntity(Trans);
		}

	}

}