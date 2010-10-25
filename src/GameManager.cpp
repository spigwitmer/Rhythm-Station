#include <stdlib.h>
#include "globals.h"
#include "FileManager.h"
#include "GameManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Logger.h"
#include "Timer.h"

GameManager* Game = NULL;
Matrix* obj = NULL;
Object* quad = NULL;

typedef std::vector<Object*> ob_vec;

GameManager::GameManager(GLFWwindow window) : m_debug(false), m_bFirstUpdate(true) {
	m_window = window;

	glfwEnable(window, GLFW_SYSTEM_KEYS); // cmd+q, alt+f4, etc.
	glfwEnable(window, GLFW_MOUSE_CURSOR); // normal mouse

	// let the OS know we're up and running.
	glfwPollEvents();

	g_projection_matrix = new Matrix();
	g_projection_matrix->LoadIdentity();

	glfwGetWindowSize(window, &m_width, &m_height);

//	g_projection_matrix->Perspective(60.f, (float)m_width/m_height, 0.1f, 500.f);
	g_projection_matrix->Ortho(-m_width/2, m_width/2, m_height/2, -m_height/2, -10, 10);
}

GameManager::~GameManager() {
	this->Cleanup();
	delete g_projection_matrix, obj, quad;
}

GLFWwindow GameManager::GetWindow() {
	return m_window;
}

void GameManager::QueueRendering() {
	m_bQueuedRender = true;
}

void GameManager::Cleanup() { }

void GameManager::Update(double delta) {
	m_objects = Resources->GetObjects();
	for (int i = 0; i<m_objects.size(); i++)
		m_objects[i]->Update(delta);
}

void GameManager::Start() {
	Scene->PushScreen();
	quad = new Object();
	quad->Translate(vec3(0,0,-10));
	std::string path = File->GetFile("test.png");
	if (File->FileExists(path))
		quad->Load(path);
	quad->Register();

	glClearColor(0.25, 0.25, 0.25, 1.0);
}

void GameManager::Render() {
	if (!m_bQueuedRender && !m_bFirstUpdate)
		return;
	if (m_bFirstUpdate)
		m_bFirstUpdate = false;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	Scene->Draw();

	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glfwSwapBuffers();

	// done rendering, don't do it again until requested.
	m_bQueuedRender = false;
}
