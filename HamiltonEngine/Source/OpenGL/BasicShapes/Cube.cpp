#include <PrecompiledHeader/Pch.h>

#include "Cube.h"

namespace OpenGL::BasicShapes
{
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
	}
};