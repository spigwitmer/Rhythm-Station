#include "Mesh.h"

void Mesh::Load(MeshData *data, unsigned *indices, unsigned n_verts, unsigned n_indices)
{
	bool gen_indices = false;

	if (indices == NULL)
		gen_indices = true;

	glGenBuffers(2, buffers);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MeshData) * n_verts, &data[0].Position.x, GL_STATIC_DRAW);

	// Generate indices if none exist, otherwise upload normally.
	if (gen_indices)
	{
		// If we don't have an index buffer, it's probably just 0-n
		indices = new unsigned[n_verts];
		for (unsigned i = 0; i<n_verts; i++)
			indices[i] = i;
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * n_indices, &indices[0], GL_STATIC_DRAW);

	if (gen_indices)
		delete[] indices;

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

	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	// todo: set pointers (tends to be needed)

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);

	glDrawElements(GL_TRIANGLES, num_verts, GL_UNSIGNED_INT, NULL);
}
