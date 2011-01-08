#include <GL/glfw3.h>
#include "Application.h"
#include "Logger.h"
#include "Window.h"

void Application::Init()
{
	glfwInit();

	// Initialize globals.
	Log = new Logger();
}

void Application::Update(double delta)
{
	// TODO
}

int Application::Run()
{
	int status = 0;

	double now, then;
	now = then = glfwGetTime();

	while (true)
	{
		// Work out time since last frame.
		now = glfwGetTime();
		double delta = now - then;
		then = now;

		// Update logic
		Update(delta);

		// Update input
		glfwPollEvents();
		if (glfwGetKey(Window::GetWindow(), GLFW_KEY_ESC))
			break;

		// Draw
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// TODO: draw stuff.

		glfwSwapBuffers();
	}

	return status;
}

void Application::StartListening(std::string event, MessageListener* scr)
{
	vpEventListeners[event].push_back(scr);
}

void Application::StopListening(std::string event, MessageListener* scr)
{
	// cut down some typing.
	std::vector<MessageListener*> listeners = vpEventListeners[event];

	// find the screen and remove it.
	for (size_t i = listeners.size(); i>0; --i)
	{
		if (listeners[i] == scr)
		{
			listeners.erase(listeners.begin()+i);
			break;
		}
	}
}

void Application::DispatchMessage(Message &msg)
{
	std::vector<MessageListener*> listeners = vpEventListeners[msg.name];

	for (size_t i = 0; i<listeners.size(); i++)
		listeners[i]->HandleMessage(msg);
}
