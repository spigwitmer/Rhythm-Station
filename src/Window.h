#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <GL/glfw3.h>

namespace Window
{
	bool Create(int width, int height, bool fullscreen = false);
	void Destroy();

	bool GetFocus();

	GLFWwindow GetWindow();
}

#endif
