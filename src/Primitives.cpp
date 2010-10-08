#include "Primitives.h"
#include "MathUtils.h"
#include <GL/glfw.h>
#include <math.h>

// TODO: make some functions for building vertex arrays / VBOs

void Primitive::Ngon(float _radius, unsigned _edges)
{
	std::vector<Vertex> verts;
	verts.push_back(Vertex(0.f,0.f,0.f));

	float step = 360 / float(_edges);
	for (int i = _edges; i >= 0; i--) // reversed so that it faces the correct direction.
	{
		float angle = radf(i*step);
		float x = cosf(angle)*_radius;
		float y = sinf(angle)*_radius;
		verts.push_back(Vertex(x,y,0.0f));
	}
	glEnableClientState(GL_VERTEX_ARRAY);	
	glVertexPointer(3, GL_FLOAT, 0, &verts[0].x);
	glDrawArrays(GL_TRIANGLE_FAN, 0, verts.size());
	glDisableClientState(GL_VERTEX_ARRAY);
}

// This is primarily for testing - use an Actor for real usage.
void Primitive::Quad(vec2 size)
{
	float cx = size.x * 0.5f;
	float cy = size.y * 0.5f;

	std::vector<Vertex> verts;
	std::vector<TexCoord> coords;

	verts.push_back(Vertex(-cx, -cy, 0.0f));
	coords.push_back(TexCoord(0.0f, 0.0f));

	verts.push_back(Vertex(-cx, cy, 0.0f));
	coords.push_back(TexCoord(0.0f, 1.0f));

	verts.push_back(Vertex(cx, -cy, 0.0f));
	coords.push_back(TexCoord(1.0f, 0.0f));

	verts.push_back(Vertex(cx, cy, 0.0f));
	coords.push_back(TexCoord(1.0f, 1.0f));

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, &verts[0].x);
	glTexCoordPointer(2, GL_FLOAT, 0, &coords[0].u);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, verts.size());

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}
