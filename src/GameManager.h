#ifndef _GAME_MANAGER_
#define _GAME_MANAGER_

#include <GL/glfw3.h>
#include <vector>
#include <map>
#include "Matrix.h"
#include "InputManager.h"
#include "Screen.h"

class GameManager
{
public:
	GameManager(GLFWwindow window);
	virtual ~GameManager();

	GLFWwindow GetWindow();
	void SetWindowActive() { m_window_active = glfwGetWindowParam(m_window, GLFW_ACTIVE); }
	bool IsWindowActive() { return m_window_active; }

	void SetDebugMode(bool debug) { m_debug = debug; }
	bool IsDebugMode() { return m_debug; }

	void SetCurrentShader(GLuint _shader) { current_shader = _shader; }
	GLuint GetCurrentShader() { return current_shader; }
	void UpdateWindowTitle(double delta);

	void Start();

	Screen* GetTopScreen() { return vpScreens.back(); }
	void SendInput(const IEvent &e);
	void Update(double delta);

	void QueueRendering();
	void Render();
	
	Matrix* ProjectionMatrix;

	std::map<std::string, bool> IsExtSupported;

	std::string GetExtensions();
	
	void CheckExtensions();
	
	int ScreenWidth, ScreenHeight;

private:
	GLFWwindow m_window;
	GLuint current_shader;
	bool m_debug, m_bFirstUpdate, m_bQueuedRender, m_window_active;

	std::vector<Screen*> vpScreens;
};

extern GameManager* Game;

#endif
