#ifndef _RENDER_MANAGER_H_
#define _RENDER_MANAGER_H_

#include <GL/glfw3.h>
#include <string>
#include <map>
#include "Shader.h"
#include "Type.h"

class RenderManager
{
public:
	RenderManager();
	virtual ~RenderManager() { }

	std::string GetExtensions();

	void CheckExtensions();

	int ScreenWidth, ScreenHeight;
};

extern RenderManager* Renderer;

#endif
