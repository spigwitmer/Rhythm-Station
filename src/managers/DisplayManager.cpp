#include <GL3/gl3w.h>
#include <GL/glfw3.h>
#include "DisplayManager.h"
#include "utils/Logger.h"
#include <string>
#include <renderer/common/Error.h>

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
	glfwOpenWindowHint(GLFW_WINDOW_RESIZABLE, 1);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		
	// First, try to create a GL 3.2 context
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
	m_window = glfwOpenWindow(854, 480, GLFW_WINDOWED, "Rhythm-Station", NULL);
	glfwSwapInterval(0);

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
