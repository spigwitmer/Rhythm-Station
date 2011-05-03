#include <GL/glew.h>
#include <GL/glfw3.h>
#include "Message.h"
#include "utils/Logger.h"
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
	Window *wnd = Window::getSingleton();
	glfwSetWindowTitle(wnd->getWindow(), Log->SPrint("%dx%d", width, height).c_str());
	
	// Set new size preference (should this only be saved explicitely by user?)
	Preferences->SetValue("Graphics", "WindowWidth", width);
	Preferences->SetValue("Graphics", "WindowHeight", height);
	
	Message msg;
	msg.name = "WindowResized";
	msg.Send();
	
	glViewport(0, 0, width, height);
		
	/*
	 * Work out aspect ratio and set game resolution accordingly. Having
	 * this based on 640x480 is much less work for themers. If they want
	 * the actual size in pixels, it's easy enough to scale back to the
	 * real size. A base_size square is guaranteed usable.
	 */
	float base_size = 480;
	vec2 game_size = vec2(0.0);
	
	// Wide screen (e.g. computer monitor)
	vec2 ws = vec2(width, height);
	if (ws.x >= ws.y) {
		game_size.x = base_size * (ws.x/ws.y);
		game_size.y = base_size;
	}
	// Tall screen (e.g. tablet, phone)
	else {
		game_size.x = base_size;
		game_size.y = base_size * (ws.y/ws.x);
	}
	Game->ProjectionMatrix->Ortho(game_size.x, game_size.y);
	
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
	bool windowfocus = glfwGetWindowParam(this->getWindow(), GLFW_ACTIVE) ? true : false;
	return windowfocus || is_fullscreen;
}

void Window::UpdateTitle(double delta)
{
	glfwSetWindowTitle(this->getWindow(), Log->SPrint("Rhythm-Station - FPS: %0.1f, Delta: %0.05f",
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


/**
 * @file
 * @author Colby Klein (c) 2011
 * @section LICENSE
 * 
 * All rights reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, and/or sell copies of the Software, and to permit persons to
 * whom the Software is furnished to do so, provided that the above
 * copyright notice(s) and this permission notice appear in all copies of
 * the Software and that both the above copyright notice(s) and this
 * permission notice appear in supporting documentation.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF
 * THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS
 * INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
