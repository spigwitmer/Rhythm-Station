#include "ResourceManager.h"
#include "Object.h"

ResourceManager* Resources = NULL;

void ResourceManager::CheckDuplicate(std::string path) {
	
}

void ResourceManager::Add(Object* object) {
	m_objects.push_back(object);
}

void ResourceManager::Add(Shader* shader) {
	m_shaders.push_back(shader);
}

void ResourceManager::Add(Texture* texture) {
	m_textures.push_back(texture);
}

std::vector<Object*> ResourceManager::GetObjects() {
	return m_objects;
}

template <class T>
void ResourceManager::Reload(std::vector<T*> obj) {
	for (unsigned int i = 0; i<obj.size(); i++)
		obj[i]->Reload();
}

template <class T>
void ResourceManager::Delete(std::vector<T*> obj) {
	for (unsigned int i = 0; i<obj.size(); i++)
		delete obj[i];
}

void ResourceManager::ReloadAll() {
	Reload<Shader> (m_shaders);
}

void ResourceManager::ClearObjects() {
	Delete<Object> (m_objects);
}

void ResourceManager::ClearAll() {
	Delete<Shader> (m_shaders);
	Delete<Object> (m_objects);
	Delete<Texture> (m_textures);
}
