#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <GL/glfw3.h>
#include <string>

struct Texture
{
	Texture();
	void Delete();
	void Bind();
	void Unbind();
	void Load(std::string path);
	
	GLuint ptr;
	std::string path;
	unsigned int width, height;
	unsigned int refcount;
};

#endif
