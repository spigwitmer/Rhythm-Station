#include <vector>
#include "ResourceManager.h"
#include "drawables/Object.h"
#include "utils/Logger.h"

// all the scene stuff.
std::vector<Object *> m_objects;
std::vector<Texture> m_textures;
std::vector<Shader *> m_shaders;

void ResourceManager::Load(std::string path)
{
	using namespace std;
	size_t pos = path.find_last_of(".");
	
	if (pos != string::npos)
	{
		string ext = path.substr(pos, path.length());
		Log->Print(ext);
	}
}

bool ResourceManager::GetResource(std::string path, Texture &texture)
{
	std::vector<Texture>::reverse_iterator rit;
	
	for (rit = m_textures.rbegin(); rit < m_textures.rend(); ++rit)
	{
		if (path.compare((*rit).path) == 0)
		{
			rit->refcount++;
			texture = *rit;
			return true;
		}
	}
	
	return false;
}

bool ResourceManager::GetResource(std::string path, Shader *shader)
{
	// TODO
	return false;
}

void ResourceManager::FreeResource(Texture texture)
{
	std::vector<Texture>::iterator it;
	
	for (it = m_textures.begin(); it < m_textures.end(); ++it)
	{
		if (texture.path.compare((*it).path) == 0)
			if (--it->refcount == 0)
				m_textures.erase(it);
	}
}

void ResourceManager::Add(Shader *shader)
{
	m_shaders.push_back(shader);
}

void ResourceManager::Add(Texture texture)
{
	m_textures.push_back(texture);
}

template <class T>
void ResourceManager::Reload(std::vector<T *> obj)
{
	for (unsigned int i = 0; i<obj.size(); i++)
		obj[i]->Reload();
}

template <class T>
void ResourceManager::Delete(std::vector<T *> obj)
{
	for (unsigned int i = 0; i<obj.size(); i++)
		delete obj[i];
}

void ResourceManager::ReloadAll()
{
	Reload<Shader> (m_shaders);
}

void ResourceManager::ClearAll()
{
	Delete<Shader> (m_shaders);
	Delete<Object> (m_objects);
}


/**
 * @file
 * @author Colby Klein, Jarno Purontakanen (c) 2011
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
