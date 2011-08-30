#pragma once

#include <GL/glfw3.h>
#include "utils/Logger.h"

class DisplayManager
{
public:
	DisplayManager();
	void Init(bool using_gl3);
	void CheckError();
	void Flush();
	bool OpenWindow(GLFWwindow &wnd);
	
	int max_anisotropy;
	int max_attributes;
	int max_uniforms;
	int max_texture_size;
};

/**
 * Colby Klein (c) 2011
 * Licensed under the terms of the MIT license. See License.txt.
 */
