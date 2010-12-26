#ifndef _GAME_MANAGER_
#define _GAME_MANAGER_

#include <GL/glfw3.h>
//#include <vector>
#include <map>
#include "Matrix.h"

class GameManager
{
public:
	GameManager(GLFWwindow window);
	virtual ~GameManager();

	GLFWwindow GetWindow();

	bool IsDebugMode() { return m_debug; }
	void SetDebugMode(bool debug) { m_debug = debug; }
	void SetCurrentShader(GLuint _shader) { current_shader = _shader; }
	GLuint GetCurrentShader() { return current_shader; }
	void SetActive(bool active) { m_window_active = active; }
	void UpdateWindowTitle(double delta);

	void Start();
	void QueueRendering();
	void Update(double delta);
	void Render();

	Matrix* ProjectionMatrix;

	std::map<std::string, bool> IsExtSupported;

private:
	GLFWwindow m_window;
	GLuint current_shader;
	bool m_debug, m_bFirstUpdate, m_bQueuedRender, m_window_active;
};

extern GameManager* Game;

#endif
