#include <GL/glew.h>
#include <stdlib.h>
#include "SoundManager.h"
#include "GameManager.h"
#include "InputManager.h"
#include "utils/Logger.h"
#include "utils/PreferencesFile.h"
#include "drawables/Object.h"
#include "utils/Timer.h"
#include <iostream>
#include "Type.h"

#if defined(_WIN32) || defined(__WINDOWS__)
#include <windows.h>
#define usleep(x) Sleep((x) / 1000)
#endif

GameManager *Game = NULL;
bool lazy_updates = true;

GameManager::GameManager(GLFWwindow window) : m_bFirstUpdate(true)
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

//#ifdef DEBUG
	lazy_updates = false;
//#endif
	ProjectionMatrix = new Matrix();
	
	Log->Print("Current Theme: %s", Preferences->GetValue("Game", "CurrentTheme", "default"));
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

GLuint GameManager::getCurrentShader()
{
	return mShaderID;
}

void GameManager::setCurrentShader(GLuint shader)
{
	mShaderID = shader;
}

void GameManager::Update(double delta)
{
	std::vector<Object*>::iterator it;
	std::vector<Sound*>::iterator it2;
	
	for (it = vpObjects.begin(); it != vpObjects.end(); it++)
		(*it)->Update(delta);
	
	for (it2 = vpSounds.begin(); it2 != vpSounds.end(); it2++)
		(*it2)->Update(delta);
}

// this should probably be in scenemanager.
void GameManager::Render()
{
	Window *wnd = Window::GetSingleton();
	/*
	 * Ideally, we would only redraw objects which need it, however this is still
	 * useful when there isn't much going on.
	 */
	if ((!m_bQueuedRender && !m_bFirstUpdate && lazy_updates) ||
		(!wnd->CheckFocus() && !m_bFirstUpdate))
	{
		usleep(12500); // reduce CPU usage when not updating.
		return;
	}
	
	if (m_bFirstUpdate)
		m_bFirstUpdate = false;
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	std::vector<Object*>::iterator it;
	for (it = vpObjects.begin(); it != vpObjects.end(); it++)
		(*it)->Draw();
	
	glfwSwapBuffers();
	
	// done rendering, don't do it again until requested.
	m_bQueuedRender = false;
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
