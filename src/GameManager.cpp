#include <stdlib.h>
#include "globals.h"
#include "GameManager.h"
#include "SceneManager.h"
#include "Logger.h"
#include "Object.h"
#include "Timer.h"
#include <iostream>

GameManager* Game = NULL;
Matrix* obj = NULL;
Object* quad = NULL;

GameManager::GameManager(GLFWwindow window) :
	current_shader(0), m_debug(false), m_bFirstUpdate(true),
	m_window_active(true) {
	m_window = window;

	glfwEnable(window, GLFW_SYSTEM_KEYS); // cmd+q, alt+f4, etc.
	glfwEnable(window, GLFW_MOUSE_CURSOR); // normal mouse

	// let the OS know we're up and running.
	glfwPollEvents();

	g_projection_matrix = new Matrix();
}

GameManager::~GameManager() {
	delete g_projection_matrix, obj, quad;
}

GLFWwindow GameManager::GetWindow() {
	return m_window;
}

void GameManager::QueueRendering() {
	m_bQueuedRender = true;
}

void GameManager::Update(double delta) {
	std::vector<Object*> vpObjects = Resources->GetObjects();
	for (int i = 0; i<vpObjects.size(); i++)
		vpObjects[i]->Update(delta);
}

void GameManager::Start() {
	Scene->PushScreen();
	glClearColor(0.25, 0.25, 0.25, 1.0);
}

void GameManager::Render() {
	/*
	 * Ideally, we would only redraw objects which need it, however this is still
	 * useful when there isn't much going on.
	 * TODO: also slow down when program is in the background.
	 */
	if (!m_bQueuedRender && !m_bFirstUpdate || !m_window_active) {
		usleep(2500); // reduce CPU usage when not updating.
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
