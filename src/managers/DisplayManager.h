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
	bool OpenWindow();
	bool IsFocused();
	GLFWwindow GetWindow();
	
	std::string GetInfoLog(GLuint obj);
	GLint GetMaximum(GLenum token);

private:
	void GetGLValue(GLint *target, GLenum param, std::string text);
	GLFWwindow m_window;
	std::map<GLenum, GLint> m_attribs;
};

/**
 * Colby Klein (c) 2011
 * Licensed under the terms of the MIT license. See License.txt.
 */
