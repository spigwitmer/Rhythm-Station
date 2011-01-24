#include <GL/glew.h>
#include <GL/glfw3.h>
#include "Message.h"
#include "Logger.h"
#include "Window.h"
#include "PreferencesFile.h"
#include "Type.h"

GLFWwindow hwnd = 0;
bool focused = false;

bool Window::Create(int width, int height, bool fullscreen)
{	
	// AA makes things pretty
	if (Preferences->GetBoolValue("Graphics", "AntiAliasing"))
		glfwOpenWindowHint(GLFW_FSAA_SAMPLES, Preferences->GetLongValue("Graphics", "AntiAliasingSamples"));
	
	// Make sure we aren't defaulting to something useless.
	glfwOpenWindowHint(GLFW_ALPHA_BITS, 8);
	glfwOpenWindowHint(GLFW_DEPTH_BITS, 32);
	int fs = fullscreen ? GLFW_FULLSCREEN : GLFW_WINDOWED;
	hwnd = glfwOpenWindow(width, height, fs, "", NULL);
	
	glewInit();
	
	int status = glfwGetError();
	if (status != GLFW_NO_ERROR)
		Log->Print(glfwErrorString(status));
	
	if (!hwnd)
		return false;
	
	Message msg;
	msg.name = "WindowOpened";
	msg.data["Size"] = (void *)vec2(width, height);
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
