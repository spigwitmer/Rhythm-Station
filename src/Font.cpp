#include "Font.h"

void Font::Load(std::string path)
{
	Object::Load(path);
	Set("");
}

void Font::Set(std::string str)
{
	m_text = str;
	
	for (int i = 0; i<str.size(); i++)
	{
		// TODO: Font crap.
	}
	
	// TODO: Update buffers
}
