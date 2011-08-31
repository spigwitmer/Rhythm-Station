#include <GL3/gl3w.h>
#include <GL/glfw3.h>
#include "DisplayManager.h"
#include "utils/Logger.h"
#include <string>

using namespace std;

DisplayManager::DisplayManager()
{
}

DisplayManager::~DisplayManager()
{
	CloseWindow();
}

void DisplayManager::GetGLValue(GLint *target, GLenum param, string text)
{
	glGetIntegerv(param, target);
	LOG->Info("- %s: %d.", text.c_str(), *target);
	CheckError();
}

string DisplayManager::GetInfoLog(GLuint obj)
{
	string log;
	GLint status, count;
	GLchar *error;
	GLenum which = glIsShader(obj) ? GL_COMPILE_STATUS : GL_LINK_STATUS;
	
	void (*GetProg)(GLuint, GLenum, GLint*);
	void (*GetProgInfo)(GLuint, GLsizei, GLsizei*, GLchar*);
	
	bool swap = (bool)glIsShader(obj);
	GetProg = swap ? glGetShaderiv : glGetProgramiv;
	GetProgInfo = swap ? glGetShaderInfoLog : glGetProgramInfoLog;
	
	GetProg(obj, which, &status);
	if (!status)
	{
		GetProg(obj, GL_INFO_LOG_LENGTH, &count);
		if (count > 0)
		{
			GetProgInfo(obj, count, NULL, (error = new char[count+1]));
			log = error;
			delete[] error;
		}
	}
	
	CheckError();
	
	return log;
}

void DisplayManager::CheckError()
{
	for (GLenum err = glGetError(); err != GL_NO_ERROR; err = glGetError())
	{
		switch (err)
		{
			case GL_INVALID_ENUM: LOG->Fatal("GL_INVALID_ENUM"); break;
			case GL_INVALID_OPERATION: LOG->Fatal("GL_INVALID_OPERATION"); break;
			case GL_INVALID_VALUE: LOG->Fatal("GL_INVALID_VALUE"); break;
			case GL_OUT_OF_MEMORY: LOG->Fatal("GL_OUT_OF_MEMORY"); break;
			default: break;
		}
	}
}

void DisplayManager::CheckFramebuffer()
{
	// kind of nasty ugly
	switch (glCheckFramebufferStatus(GL_FRAMEBUFFER))
	{
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			LOG->Fatal("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			LOG->Fatal("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
			LOG->Fatal("GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			LOG->Fatal("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			LOG->Fatal("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER");
			break;
		case GL_FRAMEBUFFER_UNDEFINED:
			LOG->Fatal("GL_FRAMEBUFFER_UNDEFINED");
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED:
			LOG->Fatal("GL_FRAMEBUFFER_UNSUPPORTED");
			break;
		default:
			break;
	}
}

void DisplayManager::Flush()
{
	glfwSwapBuffers();
}

GLFWwindow DisplayManager::GetWindow()
{
	return m_window;
}

bool DisplayManager::OpenWindow()
{
	// Shared window params, we always want these.
	glfwOpenWindowHint(GLFW_DEPTH_BITS, 24);
	glfwOpenWindowHint(GLFW_STENCIL_BITS, 8);
	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 0);
	glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, 1);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	// First, try to create a GL 3.2 context
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
	m_window = glfwOpenWindow(854, 480, GLFW_WINDOWED, "Rhythm-Station", NULL);
	
	// Make sure we were able to create a rendering context.
	if (!glfwIsWindow(m_window))
	{
		LOG->Warn("Unable to create an OpenGL 3.2 context.");
		return false;
	}
	
	gl3wInit();
	gl3wIsSupported(3, 2);
		
	// make sure nothing is terribly wrong from the start.
	CheckError();

	LOG->Info("Checking hardware capabilities.");
	GetGLValue(&m_attribs[GL_MAX_TEXTURE_SIZE], GL_MAX_TEXTURE_SIZE, "Max Texture Size");
	
	return true;
}

void DisplayManager::CloseWindow()
{
	if (glfwIsWindow(m_window))
		glfwCloseWindow(m_window);
}

bool DisplayManager::IsFocused()
{
	return (bool)glfwGetWindowParam(m_window, GLFW_ACTIVE);
}

GLint DisplayManager::GetMaximum(GLenum token)
{
	return m_attribs[token];
}

/**
 * Colby Klein (c) 2011
 * Licensed under the terms of the MIT license. See License.txt.
 */
