#ifndef _RENDER_MANAGER_H_
#define _RENDER_MANAGER_H_

#include <GL/glfw3.h>
#include <string>
#include <map>
#include "Shader.h"
#include "Type.h"

class RenderManager {
public:
	RenderManager();
	virtual ~RenderManager() { }

	std::string GetExtensions();
	std::map<std::string, bool> IsExtSupported;
	void BindBuffers(GLuint* buffers, Shader* shader);

	GLuint* CreateFramebuffer(vec2 size, int samples = 1);
	void BindFramebuffer(GLuint* buffers);
	void UnbindFramebuffer();
	void DeleteFramebuffer(GLuint* buffers);
	void CheckExtensions();

	int ScreenWidth, ScreenHeight;
};

extern RenderManager* Renderer;

#endif
