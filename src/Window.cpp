#include <GL/glfw3.h>
#include "Message.h"
#include "Logger.h"
#include "Window.h"
#include "Type.h"

GLFWwindow hwnd = 0;
bool focused = false;

// internal stuff, don't need to access it elsewhere.
static void _SetFocus(GLFWwindow window, int status)
{
	focused = (status == GL_TRUE) ? true : false;

	Message msg;
	msg.name = "WindowFocused";
	msg.data["Focused"] = (void*)focused;
	msg.Send();
}

static void _SetSize(GLFWwindow window, int x, int y)
{
	Message msg;
	msg.name = "WindowResized";
	msg.data["Size"] = (void*)vec2(x, y);
	msg.Send();
}

bool Window::Create(int width, int height, bool fullscreen)
{
	// AA makes things pretty
	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);

	// Make sure we aren't defaulting to something useless.
	glfwOpenWindowHint(GLFW_ALPHA_BITS, 8);
	glfwOpenWindowHint(GLFW_DEPTH_BITS, 32);
	hwnd = glfwOpenWindow(width, height, GLFW_WINDOWED, "", NULL);

	if (!hwnd)
		return false;

	// Set callbacks.
	glfwSetWindowFocusCallback(&_SetFocus);
	glfwSetWindowSizeCallback(&_SetSize);

	Message msg;
	msg.name = "WindowOpened";
	msg.data["Size"] = (void*)vec2(width, height);
	msg.Send();

	return true;
}

void Window::Destroy()
{
	Message msg;
	msg.name = "WindowClosed";
	msg.Send();

	glfwCloseWindow(hwnd);
}

GLFWwindow Window::GetWindow()
{
	if (!hwnd)
		Log->Print("Window doesn't exist!");

	return hwnd;
}

bool Window::GetFocus()
{
	return focused;
}
