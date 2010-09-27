#include <GL/glfw.h>
#include "Video.h"

using namespace Video;

void Draw::Begin() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
}

void Draw::End() {
	glfwSwapBuffers();
}
