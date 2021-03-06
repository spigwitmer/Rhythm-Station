#ifndef _RESOURCE_MANAGER_H_
#define _RESOURCE_MANAGER_H_

#include <string>
#include <vector>
#include "drawables/Object.h"
#include "renderer/Shader.h"
#include "renderer/Texture.h"
#include "utils/Singleton.h"

class ResourceManager : public Singleton<ResourceManager>
{
public:
	void Add(Texture texture);
	void Add(Shader *shader);

	bool GetResource(std::string path, Texture &texture);
	bool GetResource(std::string path, Shader *texture);

	void Load(std::string path);
	void FreeResource(Texture texture);
	void ClearAll();
	void ReloadAll();

	template<class T> void Delete(std::vector<T*> obj);
	template<class T> void Reload(std::vector<T*> obj);

private:
	// all the scene stuff.
	std::vector<Object*> m_objects;
	std::vector<Texture> m_textures;
	std::vector<Shader*> m_shaders;
};

#endif
