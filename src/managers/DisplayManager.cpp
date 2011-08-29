#include <GL/glew.h>
#include <GL/glfw3.h>
#include "DisplayManager.h"
#include "utils/Logger.h"
#include <string>

using namespace std;

static void GetGLValue(int *target, const char *text, int param)
{
	glGetIntegerv(param, target);
	LOG->Info("-> %s: %d.", text, *target);
}

static bool ComplainAbout(string text, int check, int limit)
{
	if (check < limit)
	{
		LOG->Warn("Insufficient number of " + text + " supported. ");
		return true;
	}

	return false;
}

static void CheckFramebufferStatus()
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

void DisplayManager::CheckError()
{
	for (GLenum err = glGetError(); err != GL_NO_ERROR; err = glGetError())
	{
		switch (err)
		{
			case GL_INVALID_ENUM:
				LOG->Fatal("GL_INVALID_ENUM");
				break;
			case GL_INVALID_OPERATION:
				LOG->Fatal("GL_INVALID_OPERATION");
				break;
			case GL_INVALID_VALUE:
				LOG->Fatal("GL_INVALID_VALUE");
				break;
			case GL_OUT_OF_MEMORY:
				LOG->Fatal("GL_OUT_OF_MEMORY");
				break;
			case GL_STACK_OVERFLOW:
				LOG->Fatal("GL_STACK_OVERFLOW");
				break;
			case GL_STACK_UNDERFLOW:
				LOG->Fatal("GL_STACK_UNDERFLOW");
				break;
			default:
				break;
		}
	}
}

DisplayManager::DisplayManager()
{
	max_anisotropy = 0;
	max_attributes = 0;
	max_texture_size = 0;
	max_uniforms = 0;
}

void DisplayManager::Flush()
{
	glfwSwapBuffers();
}

bool DisplayManager::OpenWindow(GLFWwindow &m_window)
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
	m_window = glfwOpenWindow(854, 480, GLFW_WINDOWED,
		"Rhythm-Station (GL 3.2)", NULL);
	
	// if we were able to create a window, then we support GL 3.2
	bool bUsingGL3 = glfwIsWindow(m_window) ? true : false;
	
	// We weren't able to make a GL 3.2 window. Falling back to GL 2.1.
	if (!bUsingGL3) {
		LOG->Warn("Unable to use OpenGL 3.2. Falling back to 2.1");
		glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 2);
		glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 1);
		m_window = glfwOpenWindow(854, 480, GLFW_WINDOWED,
			"Rhythm-Station (GL 2.1)", NULL);
	}
	
	// Make sure we were able to create a rendering context.
	if (!glfwIsWindow(m_window))
	{
		LOG->Warn("Unable to create an OpenGL window. Check your drivers.");
		return false;
	}
	
	// We only need GLEW for legacy contexts.
	if (!bUsingGL3)
		glewInit();
	
	Init(bUsingGL3);
	
	return true;
}

void DisplayManager::Init(bool using_gl3)
{
	// Clear error bit, just in case.
	CheckError();

	bool err = false;

	LOG->Info("Checking hardware capabilities...");
	LOG->Info("Renderer: %s %s", glGetString(GL_RENDERER),
		glGetString(GL_VERSION));

	// This functionality is guaranteed if using modern OpenGL.
	if (!using_gl3) {
		if (!glewIsSupported("GL_ARB_fragment_program"))
			LOG->Warn("Fragment programs don't appear to be supported!");
	}

	/*
	 * Note: glewIsSupported can be used without calling glewInit(), as it just
	 * parses the output of glGetString(GL_EXTENSIONS).
	 */
	if (glewIsSupported("GL_EXT_texture_filter_anisotropic"))
		GetGLValue(&max_anisotropy, "Max Anisotropic Filtering", GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT);

	GetGLValue(&max_texture_size, "Max Texture Size", GL_MAX_TEXTURE_SIZE);
	GetGLValue(&max_attributes, "Max Vertex Attributes", GL_MAX_VERTEX_ATTRIBS);
	GetGLValue(&max_uniforms, "Max Uniform Components", GL_MAX_FRAGMENT_UNIFORM_COMPONENTS);

	err = ComplainAbout("Vertex Attributes", max_attributes, 8);
	err = ComplainAbout("Fragment Uniforms", max_uniforms, 512);

	if (!err)
		LOG->Info("Everything appears to be acceptable.");
}
