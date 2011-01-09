#ifndef _MESH_H_
#define _MESH_H_

#include <GL/glfw3.h>
#include "Type.h"

struct MeshData
{
	vec3 Position;
	vec3 Normal;
	vec2 TexCoord;
};

struct Mesh
{
	// Create a VBO and upload the given data.
	void Load(MeshData *data, unsigned *indices, unsigned n_verts, unsigned n_indices);
	void Free();

	void Draw();

	GLuint buffers[2];
	unsigned num_verts;

	bool loaded;
};

#endif
