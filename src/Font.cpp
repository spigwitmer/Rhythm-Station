#include "Mesh.h"
#include "Logger.h"
#include "Font.h"

void Font::Load(std::string path)
{
	Object::Load(path);
	Set("");
}

void Font::Set(std::string str)
{
	if (!mesh.loaded)
	{
		Log->Print("WARNING: Attempt to set text before loading a font!");
		return;
	}
	
	m_text = str;
	
	std::vector<MeshData> verts;
	mesh.num_verts = str.length()*4;
	
	for (int i = 0; i<str.length(); i++)
	{
		// TODO: Font crap.
	}
	
	// TODO: Update buffers
	glBindBuffer(GL_ARRAY_BUFFER, mesh.buffers[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(MeshData)*verts.size(), &verts[0].Position.x);
}
