#include <PrecompiledHeader/Pch.h>
#include "Cube.h"
#include "Configuration/Globals.h"
#include <OpenGL/OpenGL.h>
#include <OpenGL/Utils.h>

namespace HamiltonEngine::OpenGL
{
	// Returns a Vertex Array and Vertex Buffer with all the data for a unit cube 
	void CreateUnitCube(GLuint* VAO, GLuint* VBO)
	{
		glGenVertexArrays(1, VAO);
		glBindVertexArray(*VAO); // The active Vertex Array Object

		glCreateBuffers(1, VBO); 

		glBindBuffer(GL_ARRAY_BUFFER, *VBO); // Bind the Vertex Buffer object to the GL_ARRAY_BUFFER
		glBufferData(GL_ARRAY_BUFFER, sizeof(UNIT_CUBE_VERTS_AND_TEX_COORDS), UNIT_CUBE_VERTS_AND_TEX_COORDS, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0); // unbind so something else doesn't do something to this array.
	}

	entt::entity CreateCube(TransformComponent Trans)
	{
		std::cout << glGetError() << std::endl;
		std::cout << glGetError() << std::endl;
		entt::registry& reg = HamiltonEngine::Globals::Registry;
		const auto ent = reg.create();

		OpenGLBuffersComponent Buffs;
		Buffs = CreateOpenGLBuffersComponent(false);

		glBindVertexArray(Buffs.VAO);
		
		glBindBuffer(GL_ARRAY_BUFFER, Buffs.VBO);
		
		// Need to know the size of the buffer to create
		glBufferData(GL_ARRAY_BUFFER, sizeof(UNIT_CUBE_VERTS) + sizeof(UNIT_CUBE_TEXTURE_COORD), NULL, GL_STATIC_DRAW);


		glBufferSubData(GL_ARRAY_BUFFER, 0,								  sizeof(UNIT_CUBE_VERTS),		    &UNIT_CUBE_VERTS);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(UNIT_CUBE_VERTS), sizeof(UNIT_CUBE_TEXTURE_COORD),			&UNIT_CUBE_TEXTURE_COORD);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(sizeof(UNIT_CUBE_VERTS)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		Buffs.DrawMode = GL_TRIANGLES;
		Buffs.start = 0;
		Buffs.count = (sizeof(UNIT_CUBE_VERTS) / sizeof(float)) / 3; // length of array / 3points / triangle

		//PrintOpenGLBufferComponentData(Buffs);
		//PrintTransformComponent(Trans);

		reg.emplace<OpenGLBuffersComponent>(ent, Buffs);
		reg.emplace<TransformComponent>(ent, Trans);

		return ent;

	}

	void TestCubes(int num)
	{
		for (int i = 0; i < num; i++)
		{
			TransformComponent Trans;
			Trans = RandomTransformComponent();

			CreateCube(Trans);
		}
	}
};