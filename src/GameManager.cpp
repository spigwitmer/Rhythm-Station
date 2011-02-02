#include <GL/glew.h>
#include <stdlib.h>
#include "SoundManager.h"
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
	current_shader(0), m_bFirstUpdate(true)
{
	// Don't take over mouse, alt+f4/cmd+q, etc.
	glfwEnable(window, GLFW_SYSTEM_KEYS);
	glfwEnable(window, GLFW_MOUSE_CURSOR);
	
	// let the OS know we're up and running.
	glfwPollEvents();
	
	// Check for some extensions (just CSAA right now)
	if (GLEW_NV_framebuffer_multisample_coverage)
	{
		Log->Print("CSAA Supported.");
		IsExtSupported["CSAA"] = true;
	}
	else
		Log->Print("CSAA Not Supported.");
		
	// Make transparency work!
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.25, 0.25, 0.25, 1.0);

#ifdef DEBUG
	lazy_updates = false;
#endif
	ProjectionMatrix = new Matrix();
}

GameManager::~GameManager()
{	
	delete ProjectionMatrix;
}

void GameManager::AddObject(Object *obj)
{
	vpObjects.push_back(obj);
}

void GameManager::AddSound(Sound *snd)
{
	vpSounds.push_back(snd);
}

std::string GameManager::GetExtensions()
{
	std::ostringstream extensions;
	extensions << glGetString(GL_EXTENSIONS);
	return extensions.str();
}

void GameManager::QueueRendering()
{
	m_bQueuedRender = true;
}

void GameManager::Update(double delta)
{
	for (size_t i = 0; i<vpObjects.size(); i++)
		vpObjects[i]->Update(delta);
	
	for (size_t i = 0; i<vpSounds.size(); i++)
		vpSounds[i]->Update(delta);
}

// this should probably be in scenemanager.
void GameManager::Render()
{
	/*
	 * Ideally, we would only redraw objects which need it, however this is still
	 * useful when there isn't much going on.
	 */
	if ((!m_bQueuedRender && !m_bFirstUpdate && lazy_updates) ||
		(!Window::CheckFocus() && !m_bFirstUpdate))
	{
		usleep(12500); // reduce CPU usage when not updating.
		return;
	}
	
	if (m_bFirstUpdate)
		m_bFirstUpdate = false;
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	for (size_t i = 0; i<vpObjects.size(); i++)
		vpObjects[i]->Draw();
	
	glfwSwapBuffers();
	
	// done rendering, don't do it again until requested.
	m_bQueuedRender = false;
}
