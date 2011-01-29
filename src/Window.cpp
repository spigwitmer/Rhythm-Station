#include <GL/glew.h>
#include <GL/glfw3.h>
#include "Message.h"
#include "Logger.h"
#include "Window.h"
#include "PreferencesFile.h"
#include "Type.h"

GLFWwindow hwnd = 0;
bool focused = false;

int width = 1, height = 1;

bool Window::Create(int w, int h, bool fullscreen)
{	
	// AA makes things pretty
	if (Preferences->GetBoolValue("Graphics", "AntiAliasing", false))
		glfwOpenWindowHint(GLFW_FSAA_SAMPLES, Preferences->GetLongValue("Graphics", "AntiAliasingSamples", 4));
	
	// Make sure we aren't defaulting to something useless.
	glfwOpenWindowHint(GLFW_ALPHA_BITS, 8);
	glfwOpenWindowHint(GLFW_DEPTH_BITS, 32);
	int fs = fullscreen ? GLFW_FULLSCREEN : GLFW_WINDOWED;
	hwnd = glfwOpenWindow(w, h, fs, "", NULL);
	
	glewInit();
	
	int status = glfwGetError();
	if (status != GLFW_NO_ERROR)
		Log->Print(glfwErrorString(status));
	
	if (!hwnd)
		return false;
	
	Message msg;
	msg.name = "WindowOpened";
	msg.data["Size"] = (void *)vec2(w, h);
	msg.Send();
	
	width = w;
	height = h;
	
	return true;
}

void Window::Destroy()
{
	Message msg;
	msg.name = "WindowClosed";
	msg.Send();
	
	glfwCloseWindow(hwnd);
}

GLFWwindow Window::getWindow()
{
	if (!hwnd)
		Log->Print("Window doesn't exist!");
		
	return hwnd;
}

bool Window::CheckFocus()
{
	return glfwGetWindowParam(Window::getWindow(), GLFW_ACTIVE) ? true : false;
}

void Window::UpdateTitle(double delta)
{
	glfwSetWindowTitle(Window::getWindow(), Log->SPrint("Rhythm-Station - FPS: %0.1f, Delta: %0.05f",
		int((1.0 / delta) * 10) * 0.1f, delta).c_str());
}

int Window::getWidth()
{
	return width;
}

int Window::getHeight()
{
	return height;
}
