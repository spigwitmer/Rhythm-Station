#include <stdlib.h>
#include "AudioManager.h"
#include "GameManager.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "Logger.h"
#include "Object.h"
#include "Timer.h"
#include <iostream>
#include "Type.h"

#if defined(_WIN32) || defined(__WINDOWS__)
#include <windows.h>
#define usleep(x) Sleep((x) / 1000)
#endif

GameManager* Game = NULL;
Matrix* obj = NULL;
Object* quad = NULL;
bool lazy_updates = true;

GameManager::GameManager(GLFWwindow window) :
	current_shader(0), m_debug(false), m_bFirstUpdate(true),
	m_window_active(true)
{
	m_window = window;

	glfwEnable(window, GLFW_SYSTEM_KEYS); // cmd+q, alt+f4, etc.
	glfwEnable(window, GLFW_MOUSE_CURSOR); // normal mouse

	// let the OS know we're up and running.
	glfwPollEvents();

#ifdef DEBUG
	lazy_updates = false;
#endif

	ProjectionMatrix = new Matrix();
}

GameManager::~GameManager()
{
	delete ProjectionMatrix;
	delete obj;
	delete quad;
}

GLFWwindow GameManager::GetWindow()
{
	return m_window;
}

void GameManager::QueueRendering()
{
	m_bQueuedRender = true;
}

void GameManager::Start()
{
	Scene->PushScreen();
	glClearColor(0.25, 0.25, 0.25, 1.0);
}

void GameManager::UpdateWindowTitle(double delta)
{
	std::string str = Log->SPrint("Rhythm-Station - FPS: %0.1f, Delta: %0.05f",
		int((1.0 / delta) * 10) * 0.1f, delta
	);

	glfwSetWindowTitle(m_window, str.c_str());
}

// this should probably be in scenemanager.
void GameManager::Render()
{
	/*
	 * Ideally, we would only redraw objects which need it, however this is still
	 * useful when there isn't much going on.
	 */
	if ((!m_bQueuedRender && !m_bFirstUpdate && lazy_updates) || (!IsWindowActive() && !m_bFirstUpdate))
	{
		usleep(12500); // reduce CPU usage when not updating.
		return;
	}
	if (m_bFirstUpdate)
		m_bFirstUpdate = false;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Scene->Draw();

	glfwSwapBuffers();

	// done rendering, don't do it again until requested.
	m_bQueuedRender = false;
}
