#ifndef _GAME_MANAGER_
#define _GAME_MANAGER_

#include <GL/glfw3.h>
#include <vector>
#include "Object.h"

class GameManager {
public:
	GameManager(GLFWwindow window);
	virtual ~GameManager();

	GLFWwindow GetWindow();

	bool IsDebugMode() { return m_debug; }
	void SetDebugMode(bool debug) { m_debug = debug; }

	void Start();
	void QueueRendering();
	void AddObject(Object* object);
	void Update(double delta);
	void Render();

private:
	GLFWwindow m_window;
	std::vector<Object*> m_objects;
	bool m_debug, m_bFirstUpdate, m_bQueuedRender;

	int m_width, m_height;
};

extern GameManager* Game;

#endif
