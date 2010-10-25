#ifndef _RESOURCE_MANAGER_H_
#define _RESOURCE_MANAGER_H_

#include <string>
#include <vector>
#include "Shader.h"
#include "Object.h"

class PNGLoader;

struct Texture {
	void Delete() {
		glDeleteTextures(1, &ptr);
	}
	void Bind() {
		glBindTexture(GL_TEXTURE_2D, ptr);
	}
	void Unbind() {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	GLuint ptr;
	std::string path;
	int width, height;
	PNGLoader *loader;
};

class ResourceManager {
public:
	void Add(Object* object);
	void Add(Texture* texture);
	void Add(Shader* shader);
	std::vector<Object*> GetObjects();
	void CheckDuplicate(std::string path);
	void ClearObjects();
	void ClearAll();
	void ReloadAll();

private:
	template<class T> void Delete(std::vector<T*> obj);
	template<class T> void Reload(std::vector<T*> obj);

	// all the scene stuff.
	std::vector<Object*> m_objects;
	std::vector<Texture*> m_textures;
	std::vector<Shader*> m_shaders;
};

extern ResourceManager* Resources;

#endif
