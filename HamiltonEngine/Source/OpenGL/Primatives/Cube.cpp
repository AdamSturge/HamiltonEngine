#include <PrecompiledHeader/Pch.h>
#include "Cube.h"
#include "Configuration/Globals.h"
#include <OpenGL/OpenGL.h>
#include <OpenGL/Utils.h>

namespace HamiltonEngine::OpenGL
{
	HamiltonEngine::OpenGL::OpenGLBuffersComponent CreateCubeBuffers()
	{
		OpenGLBuffersComponent Buffs;
		Buffs = CreateOpenGLBuffersComponent(false);

		glBindBuffer(GL_ARRAY_BUFFER, Buffs.VBO);
		// Need to know the size of the buffer to create
		glBufferData(GL_ARRAY_BUFFER, sizeof(UNIT_CUBE_VERTS) +sizeof(UNIT_CUBE_NORMALS) + sizeof(UNIT_CUBE_TEXTURE_COORD) , NULL, GL_STATIC_DRAW);

		glBufferSubData(GL_ARRAY_BUFFER,												   0,		  sizeof(UNIT_CUBE_VERTS), &UNIT_CUBE_VERTS);
		glBufferSubData(GL_ARRAY_BUFFER,							 sizeof(UNIT_CUBE_VERTS),       sizeof(UNIT_CUBE_NORMALS), &UNIT_CUBE_NORMALS);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(UNIT_CUBE_VERTS) + sizeof(UNIT_CUBE_NORMALS), sizeof(UNIT_CUBE_TEXTURE_COORD), &UNIT_CUBE_TEXTURE_COORD);

		glBindVertexArray(Buffs.VAO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)(sizeof(UNIT_CUBE_VERTS)));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), (void*)(sizeof(UNIT_CUBE_VERTS) + sizeof(UNIT_CUBE_NORMALS)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		Buffs.DrawMode = GL_TRIANGLES;
		Buffs.start = 0;
		Buffs.count = (sizeof(UNIT_CUBE_VERTS) / sizeof(float)) / 3; // length of array / 3points / triangle

		return Buffs;
	}

	entt::entity CreateCubeEntity(TransformComponent Trans)
	{
		entt::registry& reg = HamiltonEngine::Globals::Registry;
		const auto ent = reg.create();

		OpenGLBuffersComponent Buffs = CreateCubeBuffers();

		reg.emplace<OpenGLBuffersComponent>(ent, Buffs);
		reg.emplace<TransformComponent>(ent, Trans);

		return ent;
	}

	entt::entity CreateCubeEntity()
	{
		TransformComponent trans;
		trans.Position = Eigen::Vector3f(0.0f, 0.0f, 0.0f);
		trans.RotationAngle = 0.0f;
		trans.RotationAxis = Eigen::Vector3f(0.0f, 0.0f, 0.0f);
		trans.Scale = Eigen::Vector3f(1.0f, 1.0f, 1.0f);

		return CreateCubeEntity(trans);
	}

	void TestCubes(int num)
	{
		for (int i = 0; i < num; i++)
		{
			TransformComponent Trans;
			Trans = RandomTransformComponent();

			CreateCubeEntity(Trans);
		}
	}
};