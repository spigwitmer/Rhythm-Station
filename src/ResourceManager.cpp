#include <vector>
#include "ResourceManager.h"
#include "Object.h"

// all the scene stuff.
std::vector<Object *> m_objects;
std::vector<Texture> m_textures;
std::vector<Shader *> m_shaders;

bool ResourceManager::GetResource(std::string path, Texture &texture)
{
	std::vector<Texture>::reverse_iterator rit;
	
	for (rit = m_textures.rbegin(); rit < m_textures.rend(); ++rit)
	{
		if (path.compare((*rit).path) == 0)
		{
			(*rit).usecount++;
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
			if (--(*it).usecount == 0)
				m_textures.erase(it);
	}
}

void ResourceManager::Add(Object *object)
{
	m_objects.push_back(object);
}

void ResourceManager::Add(Shader *shader)
{
	m_shaders.push_back(shader);
}

void ResourceManager::Add(Texture texture)
{
	m_textures.push_back(texture);
}

std::vector<Object *> ResourceManager::GetObjects()
{
	return m_objects;
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

void ResourceManager::ClearObjects()
{
	Delete<Object> (m_objects);
}

void ResourceManager::ClearAll()
{
	Delete<Shader> (m_shaders);
	Delete<Object> (m_objects);
}
