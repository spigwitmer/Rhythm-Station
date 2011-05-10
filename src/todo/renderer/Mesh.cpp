#include <GL/glew.h>
#include "Mesh.h"
#include "Shader.h"

void Mesh::Load(MeshData *data, unsigned *indices, unsigned n_verts, unsigned n_indices)
{
	// Delete old data if we've already loaded something.
	if (loaded)
		Free();
	
	// Decide if we need to generate our own indices.
	bool gen_indices = false;
	
	if (indices == NULL)
	{
		gen_indices = true;
		// If we don't have an index buffer, it's probably just 0..n
		indices = new unsigned[n_verts];
		
		for (unsigned i = 0; i<n_verts; i++)
			indices[i] = i;
	}
	
	// Get buffer IDs
	glGenBuffers(2, buffers);
	
	// Upload data.
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MeshData) * n_verts, &data[0].Position.x, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * n_indices, &indices[0], GL_STATIC_DRAW);
	
	if (gen_indices)
		delete[] indices;
	
	num_verts = n_indices;
	loaded = true;
}

/*
void Mesh::Update(MeshData *data, unsigned *indices)
{
	// TODO: mesh updating support
	// essentially, it's a wrapper for glBufferSubData.
}
*/

void Mesh::Free()
{
	// Don't free what doesn't exist.
	if (!loaded)
		return;
		
	glDeleteBuffers(2, buffers);
}

void Mesh::Draw()
{
	// Don't try to draw an unloaded mesh.
	if (!loaded)
		return;
	
	GLubyte stride = sizeof(GLfloat) * 8;
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
	
	glVertexAttribPointer(VERTEX_ARRAY, 3, GL_FLOAT, GL_FALSE, stride, (const GLvoid*) (sizeof(GLfloat) * (0)));
	glVertexAttribPointer(NORMAL_ARRAY, 3, GL_FLOAT, GL_FALSE, stride, (const GLvoid*) (sizeof(GLfloat) * (3)));
	glVertexAttribPointer(COORD_ARRAY, 2, GL_FLOAT, GL_FALSE, stride, (const GLvoid*) (sizeof(GLfloat) * (6)));
	
	glDrawElements(GL_TRIANGLES, num_verts, GL_UNSIGNED_INT, NULL);
}

void Mesh::LoadCube()
{
	float vertices[] =
	{
		 0.5, -0.5, -0.5, 0, 0, 1, 1, 0,
		 0.5, -0.5,  0.5, 0, 1, 0, 1, 0,
		-0.5, -0.5,  0.5, 1, 1, 0, 0, 0,
		-0.5, -0.5, -0.5, 0, 0, 1, 0, 0,
		 0.5,  0.5, -0.5, 0, 1, 0, 1, 1,
		 0.5,  0.5,  0.5, 1, 0, 0, 1, 1,
		-0.5,  0.5,  0.5, 0, 0, 1, 0, 1,
		-0.5,  0.5, -0.5, 0, 1, 0, 0, 1
	};
	unsigned indices[] = {
		4, 0, 3,
		4, 3, 7,
		2, 6, 7,
		2, 7, 3,
		1, 5, 2,
		5, 6, 2,
		0, 4, 1,
		4, 5, 1,
		4, 7, 5,
		7, 6, 5,
		0, 1, 2,
		0, 2, 3
	};
	MeshData verts[8];
	memcpy(&verts[0].Position.x, vertices, sizeof(float) * 8 * 8);
	Load(verts, indices, 8, 3 * 12);
}


/**
 * @file
 * @author Colby Klein (c) 2011
 * @section LICENSE
 * 
 * All rights reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, and/or sell copies of the Software, and to permit persons to
 * whom the Software is furnished to do so, provided that the above
 * copyright notice(s) and this permission notice appear in all copies of
 * the Software and that both the above copyright notice(s) and this
 * permission notice appear in supporting documentation.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF
 * THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS
 * INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
