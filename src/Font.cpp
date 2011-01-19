#include "Mesh.h"
#include "Font.h"

void Font::Load(std::string path)
{
	Object::Load(path);
	Set("");
}

void Font::Set(std::string str)
{
	m_text = str;
	
	std::vector<MeshData> verts;
	
	for (int i = 0; i<str.size(); i++)
	{
		// TODO: Font crap.
	}
	
	// TODO: Update buffers
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 0, 0);
}
