#include <GL/glew.h>
#include <GL/glfw3.h>
#include "Message.h"
#include "Logger.h"
#include "Window.h"
#include "PreferencesFile.h"
#include "Type.h"
#include "GameManager.h"

GLFWwindow hwnd = 0;
bool focused = false;
bool is_fullscreen = false;

int width = 1, height = 1;

// on window resize
static void _resizeCallback(GLFWwindow window, int width, int height)
{
	glfwSetWindowTitle(Window::getWindow(), Log->SPrint("%dx%d", width, height).c_str());
	
	// Set new size preference (should this only be saved explicitely by user?)
	Preferences->SetValue("Graphics", "WindowWidth", width);
	Preferences->SetValue("Graphics", "WindowHeight", height);
	
	Message msg;
	msg.name = "WindowResized";
	msg.Send();
	
	glViewport(0, 0, width, height);
	
	// Set new projection matrix, queue screen update.
	Game->ProjectionMatrix->Ortho(width, height);
	
	Game->QueueRendering();
}

bool Window::Create(int w, int h, bool fullscreen)
{
	// XXX: work around a GLFW bug.
	is_fullscreen = fullscreen;
	
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

// Set callbacks
void Window::Connect()
{
	glfwSetWindowSizeCallback(_resizeCallback);	
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
	bool windowfocus = glfwGetWindowParam(Window::getWindow(), GLFW_ACTIVE) ? true : false;
	return windowfocus || is_fullscreen;
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
