#include <GL/glew.h>
#include "Logger.h"
#include "Font.h"
#include "FileManager.h"
#include <ft2build.h>
#include FT_FREETYPE_H

void Font::Load(std::string path, int size)
{
	Log->Print("Loading font \"%s\"", path.c_str());
	
	FT_Library lib;
	FT_Face face;
	
	path = FileManager::GetFile(path);
	
	int err = FT_Init_FreeType(&lib);
	if (err)
	{
		Log->Print("Failed to initialize the FreeType library!");
		return;
	}
	// TODO: keep fonts in memory and handle with ResourceManager
	err = FT_New_Face(lib, path.c_str(), 0, &face);
	
	if (err == FT_Err_Unknown_File_Format)
	{
		Log->Print("Unknown font file format!");
		return;
	}
	else if (err)
	{
		Log->Print("Could not read font file.");
		return;
	}
	
	// 12pt font size
	err = FT_Set_Char_Size(face, 0, size*64, 0, 0);
	
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
	
	for (unsigned int i = 0; i<str.length(); i++)
	{
		// TODO: Font crap.
	}
	
	// TODO: Update buffers
	glBindBuffer(GL_ARRAY_BUFFER, mesh.buffers[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(MeshData) * verts.size(), &verts[0].Position.x);
}

void Font::DrawPrimitives()
{
	mesh.Draw();
}
