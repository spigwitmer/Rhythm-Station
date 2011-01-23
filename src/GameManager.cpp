#include <GL/glew.h>
#include <stdlib.h>
#include "AudioManager.h"
#include "GameManager.h"
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

GameManager *Game = NULL;
bool lazy_updates = true;

GameManager::GameManager(GLFWwindow window) :
	current_shader(0), m_debug(false), m_bFirstUpdate(true),
	m_window_active(true)
{
	m_window = window;
	
	// Don't take over mouse, alt+f4/cmd+q, etc.
	glfwEnable(window, GLFW_SYSTEM_KEYS);
	glfwEnable(window, GLFW_MOUSE_CURSOR);
	
	// let the OS know we're up and running.
	glfwPollEvents();
	glfwGetWindowSize(window, &ScreenWidth, &ScreenHeight);
	
	// Check for some extensions (just CSAA right now)
	glewInit();
	
	if (GLEW_NV_framebuffer_multisample_coverage)
	{
		Log->Print("CSAA Supported.");
		Game->IsExtSupported["CSAA"] = true;
	}
	else
		Log->Print("CSAA Not Supported.");
		
	// Make transparency work!
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#ifdef DEBUG
	lazy_updates = false;
#endif
	ProjectionMatrix = new Matrix();
}

GameManager::~GameManager()
{
	while (!vpScreens.empty())
	{
		delete vpScreens.back();
		vpScreens.pop_back();
	}
	
	delete ProjectionMatrix;
}

std::string GameManager::GetExtensions()
{
	std::ostringstream extensions;
	extensions << glGetString(GL_EXTENSIONS);
	return extensions.str();
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
	vpScreens.push_back(new Screen());
	glClearColor(0.25, 0.25, 0.25, 1.0);
}

void GameManager::Update(double delta)
{
	for (size_t i = 0; i<vpScreens.size(); i++)
		vpScreens[i]->Update(delta);
}

void GameManager::UpdateWindowTitle(double delta)
{
	glfwSetWindowTitle(m_window, Log->SPrint("Rhythm-Station - FPS: %0.1f, Delta: %0.05f",
	                   int((1.0 / delta) * 10) * 0.1f, delta).c_str());
}

void GameManager::SendInput(const IEvent &e)
{
	for (size_t i = 0; i<vpScreens.size(); i++)
		vpScreens[i]->Input(e);
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
	
	for (size_t i = 0; i<vpScreens.size(); i++)
		vpScreens[i]->Draw();
		
	glfwSwapBuffers();
	// done rendering, don't do it again until requested.
	m_bQueuedRender = false;
}
