#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <GL/glfw3.h>

struct Texture
{
	Texture() : ptr(0),usecount(1) { }
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
	unsigned int width, height;
	unsigned int usecount;
};

#endif
