#include <GL/glew.h>
#include <GL/glfw.h>
#include "Video.h"

using namespace Video;

void Window::Open()
{
	glfwOpenWindow(960, 600, 8, 8, 8, 8, 8, 0, GLFW_WINDOW);

	// do this immediately after creating the context.
	glewInit();

	glfwDisable(GLFW_AUTO_POLL_EVENTS);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
}

void Window::Close()
{
	glfwCloseWindow();
}

void Window::PollEvents()
{
	glfwPollEvents();
}
