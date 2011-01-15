#ifndef _RESOURCE_MANAGER_H_
#define _RESOURCE_MANAGER_H_

#include <string>
#include <vector>
#include "Shader.h"

// some forward-declarations because GCC has issues.
class Object;

struct Texture
{
	Texture() : ptr(0),usecount(1) { }
	void Delete() { glDeleteTextures(1, &ptr); }
	void Bind() { glBindTexture(GL_TEXTURE_2D, ptr); }
	void Unbind() { glBindTexture(GL_TEXTURE_2D, 0); }
	GLuint ptr;
	std::string path;
	unsigned int width, height;
	unsigned int usecount;
};

namespace ResourceManager
{
	void Add(Object* object);
	void Add(Texture texture);
	void Add(Shader* shader);
	std::vector<Object*> GetObjects();
	bool GetResource(std::string path,Texture& texture);
	bool GetResource(std::string path,Shader* texture);
	void FreeResource(Texture texture);
	void ClearObjects();
	void ClearAll();
	void ReloadAll();

	template<class T> void Delete(std::vector<T*> obj);
	template<class T> void Reload(std::vector<T*> obj);
};

#endif
