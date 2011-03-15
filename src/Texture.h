#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <GL/glfw3.h>
#include <string>

struct Texture
{
	Texture() : ptr(0), refcount(1) { }
	void Delete() {
		glDeleteTextures(1, &ptr);
	}
	void Bind() {
		glBindTexture(GL_TEXTURE_2D, ptr);
	}
	void Unbind() {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	void Load(std::string path);
	GLuint ptr;
	std::string path;
	unsigned int width, height;
	unsigned int refcount;
};

#endif
