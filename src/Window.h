#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <GL/glfw3.h>
#include "utils/Singleton.h"

class Window : public Singleton<Window>
{
public:
	bool Create(int width, int height, bool fullscreen = false);
	void Connect();
	void Destroy();
	
	bool CheckFocus();
	void UpdateTitle(double delta);
	
	int getWidth();
	int getHeight();
	
	GLFWwindow getWindow();
};

#endif
