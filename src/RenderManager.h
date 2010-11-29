#ifndef _RENDER_MANAGER_H_
#define _RENDER_MANAGER_H_

#include <string>
#include <map>
#include "Shader.h"
#include <GL/glfw3.h>

class RenderManager {
public:
	RenderManager();
	virtual ~RenderManager() { }

	std::string GetExtensions();
	std::map<std::string, bool> IsExtSupported;
	void BindBuffers(GLuint* buffers, Shader* shader);

	GLuint* CreateFramebuffer(int samples = 1);
	void BindFramebuffer(GLuint* buffers);
	void DeleteFramebuffer(GLuint* buffers);

private:
	void CheckExtensions();
};

extern RenderManager* Renderer;

#endif
