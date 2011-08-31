#pragma once

#include <GL/glfw3.h>
#include "utils/Logger.h"
#include <string>
#include <map>

class DisplayManager
{
public:
	DisplayManager();
	void Init(bool using_gl3);
	void CheckError();
	void Flush();
	bool OpenWindow(GLFWwindow &wnd);
	std::string GetInfoLog(GLuint obj);
	GLint GetMaximum(GLenum token);

private:
	void GetGLValue(GLint *target, GLenum param, std::string text);

	std::map<GLenum, GLint> m_attribs;
};

/**
 * Colby Klein (c) 2011
 * Licensed under the terms of the MIT license. See License.txt.
 */
