#include <PrecompiledHeader/Pch.h>
#include <Configuration/Globals.h>
#include <OpenGL/OpenGL.h>
#include <OpenGL/Utils.h>
#include "Shapes.h"
#include <Eigen/Dense>

namespace HamiltonEngine::OpenGL
{
	entt::entity CreateTetra(TransformComponent trans)
	{
		entt::registry& reg = HamiltonEngine::Globals::Registry;
		const auto ent = reg.create();

		OpenGLBuffersComponent buffs = CreateOpenGLBuffersComponent(true);
;
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

		reg.emplace<OpenGLBuffersComponent>(ent, buffs);
		reg.emplace<TransformComponent>(ent, trans);

		return ent;

	}

	entt::entity CreateSphere(TransformComponent trans, int Radius, int StackCount, int SectorCount)
	{
		entt::registry& reg = HamiltonEngine::Globals::Registry;
		const auto ent = reg.create();

		OpenGLBuffersComponent buffs = CreateOpenGLBuffersComponent(true);

		const float PI = acos(-1.0f);

		std::vector<GLfloat> Verts;
		std::vector<GLfloat> Normals;
		std::vector<GLfloat> TextCoords;

		std::vector<unsigned int> indices;
		std::vector<unsigned int> lineIndices;
		std::vector<GLfloat> InterleavedVerts;
		std::vector<GLfloat> InterleavedTextCoords;
		
		float X, Y, Z, XY;
		float NX, NY, NZ, LengthInv = 1.0f;
		float S, T;

		float SectorStep = 2 * PI / SectorCount;
		float StackStep = PI / SectorCount;
		float SectorAngle, StackAngle;


		for (int i = 0; i <= StackCount; ++i)
		{
			StackAngle = PI / 2 - i * SectorStep;
			XY = Radius * cosf(StackAngle);
			Z = Radius * sinf(StackAngle);

			for (int j = 0; j <= SectorCount; ++j)
			{
				SectorAngle = j * SectorStep;

				X = XY * cosf(SectorAngle);
				Y = XY * sinf(SectorAngle);

				Verts.push_back(X);
				Verts.push_back(Y);
				Verts.push_back(Z);

				NX = X * LengthInv;
				NY = Y * LengthInv;
				NZ = Z * LengthInv;

				Normals.push_back(NX);
				Normals.push_back(NY);
				Normals.push_back(NZ);

				S = (float)j / SectorCount;
				T = (float)i / StackCount;

				TextCoords.push_back(S);
				TextCoords.push_back(T);

			}

		}

		unsigned int k1, k2;

		for (int i = 0; i < StackCount; ++i)
		{
			k1 = i * (SectorCount + 1);
			k2 = k1 + SectorCount + 1;

			for (int j = 0; j < SectorCount; ++j, ++k1, ++k2)
			{
				if (i != 0)
				{
					indices.push_back(k1);
					indices.push_back(k2);
					indices.push_back(k1 + 1);
				}

				if (i != (StackCount - 1))
				{
					indices.push_back(k1+1);
					indices.push_back(k2);
					indices.push_back(k2 + 1);
				}

				lineIndices.push_back(k1);
				lineIndices.push_back(k2);
				if (i != 0)
				{
					lineIndices.push_back(k1);
					lineIndices.push_back(k1 + 1);
				}
			}
		}

		std::size_t i, j;
		std::size_t count = Verts.size();
		for (i = 0, j = 0; i < count; i += 3, j += 2)
		{
			InterleavedVerts.push_back(Verts[i]);			// 0 3
			InterleavedVerts.push_back(Verts[i + 1]);		// 1 4
			InterleavedVerts.push_back(Verts[i + 2]);		// 2 5

			//InterleavedVerts.push_back(Normals[i]);		 
			//InterleavedVerts.push_back(Normals[i + 1]);
			//InterleavedVerts.push_back(Normals[i + 2]);
			
			InterleavedTextCoords.push_back(TextCoords[j]);
			InterleavedTextCoords.push_back(TextCoords[j + 1]);
		}


		glBindVertexArray(buffs.VAO);
		unsigned int VertBufferSize = Verts.size() * sizeof(GLfloat);
		unsigned int NormalBufferSize = Normals.size() * sizeof(GLfloat);
		unsigned int TextCoordBufferSize = TextCoords.size() * sizeof(GLfloat);
		unsigned int IndiciesBufferSize = indices.size() * sizeof(unsigned int);
		unsigned int InterleavedVerticiesSize = InterleavedVerts.size() * sizeof(GLfloat);
		unsigned int InterleavedTextCoordsSize = InterleavedTextCoords.size() * sizeof(GLfloat);

		unsigned int TotalBufferSize = VertBufferSize + TextCoordBufferSize; // TODO: Add Normals once I do lightning

		glBindVertexArray(buffs.VAO);
		glBindBuffer(GL_ARRAY_BUFFER, buffs.VBO);

		glBufferData(GL_ARRAY_BUFFER, InterleavedVerticiesSize + InterleavedTextCoordsSize, NULL, GL_STATIC_DRAW);

		glBufferSubData(GL_ARRAY_BUFFER, 0, InterleavedVerticiesSize, InterleavedVerts.data());
		// Normals will go in here
		glBufferSubData(GL_ARRAY_BUFFER, InterleavedVerticiesSize, InterleavedTextCoordsSize, InterleavedTextCoords.data());

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffs.EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndiciesBufferSize, indices.data(), GL_STATIC_DRAW);

		buffs.DrawMode = GL_TRIANGLES;
		buffs.start = 0;
		buffs.count = InterleavedVerts.size();

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)VertBufferSize);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

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

			CreateTetra(Trans);
		}

	}

}