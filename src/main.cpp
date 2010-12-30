// GL headers
#include <GL/glfw3.h>

#include "HandleArguments.h"

// All the singletons (we init them here, should do static init?)
#include "AudioManager.h"
#include "FileManager.h"
#include "GameManager.h"
#include "InputManager.h"
#include "LuaManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "RenderManager.h"
#include "Logger.h"

#include "Thread.h"

// temp
void *print_stuff(void *arg)
{
	Thread* t = (Thread*)arg;

	// do stuff

	// lock for whatever reason
	t->Lock();

	// sync some data

	// and unlock again
	t->Unlock();
	return NULL;
}

void test_threads()
{
	int n_threads = 10;
	Thread threads[n_threads];
	for (int i = 0; i<n_threads; i++)
		threads[i].Create(&print_stuff);
}

#ifndef DEBUG
#ifdef _WIN32
// TODO: Different main.
#endif
#endif
// Initialize everything and set up the GL states used throughout the program.
int main (int argc, char** argv)
{
	glfwInit();

	GLFWvidmode mode;
	glfwGetDesktopMode(&mode);
	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4); // 4x MSAA
	glfwOpenWindowHint(GLFW_DEPTH_BITS, 32);
	GLFWwindow window = glfwOpenWindow(854, 480, GLFW_WINDOWED, "", NULL);
	glfwSwapInterval(1);

	glEnable(GL_DEPTH_TEST);

	// Start up all our singletons.
	Log			= new Logger();
	File		= new FileManager();
	Resources	= new ResourceManager();
	Game		= new GameManager(window);
	Scene		= new SceneManager();
	Audio		= new AudioManager();
	Input		= new InputManager();
	Renderer	= new RenderManager();
	Lua			= new LuaManager();

	// Handle the arguments before doing anything else
	HandleArguments(argc, argv);

	Audio->Open();

	// Start running Lua and begin the first screen.
	Game->Start();
	Lua->Start();

	// cube
	GLfloat verts[] = {
		 1, -1, -1, 0, 0, 1, 0, 0,
		 1, -1,  1, 0, 1, 0, 0, 0,
		-1, -1,  1, 1, 0, 0, 0, 0,
		-1, -1, -1, 0, 0, 1, 0, 0,
		 1,  1, -1, 0, 1, 0, 0, 0,
		 1,  1,  1, 1, 0, 0, 0, 0,
		-1,  1,  1, 0, 0, 1, 0, 0,
		-1,  1, -1, 0, 1, 0, 0, 0,
	};
	GLubyte indices[] = {
		4, 0, 3,
		4, 3, 7,
		2, 6, 7,
		2, 7, 3,
		1, 5, 2,
		5, 6, 2,
		0, 4, 1,
		4, 5, 1,
		4, 7, 5,
		7, 6, 5,
		0, 1, 2,
		0, 2, 3
	};

	GLuint vbo[2];
	GLubyte stride = sizeof(GLfloat) * 8;
	int numverts = sizeof(indices) / sizeof(GLubyte);

	glGenBuffers(2, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// vertices
	glEnableVertexAttribArray(VERTEX_ARRAY);
	glVertexAttribPointer(VERTEX_ARRAY, 3, GL_FLOAT, GL_FALSE, stride, (const GLvoid*) (sizeof(GLfloat) * (0)));

	// normals
	glEnableVertexAttribArray(NORMAL_ARRAY);
	glVertexAttribPointer(NORMAL_ARRAY, 3, GL_FLOAT, GL_FALSE, stride, (const GLvoid*) (sizeof(GLfloat) * (3)));

	// coords
	glEnableVertexAttribArray(COORD_ARRAY);
	glVertexAttribPointer(COORD_ARRAY, 2, GL_FLOAT, GL_FALSE, stride, (const GLvoid*) (sizeof(GLfloat) * (6)));

	Object* obj = new Object();
	obj->DepthClear(true);
	obj->AssignBuffer(vbo, numverts);
	obj->Scale(vec3(100));
	obj->Translate(vec3(0,125,-100));
	obj->Rotate(vec3(45));

	test_threads();

	double then = glfwGetTime(); // prevent registering a skip on first update
	double max_delta = (1.0/60.0) * 3.0;
	while (glfwIsWindow(window))
	{
		double now = glfwGetTime();
		double delta = fabs(now - then);

		/*
		 * Check that the window is active and update the title twice per second.
		 * Do this before limiting the delta so it always reports the true value.
		 */
		if (int(then * 2) != int(now * 2)) {
			Game->UpdateWindowTitle(delta);
			Game->SetWindowActive();
		}

		// just a test.
		float rot = sinf(now) * 45;
		obj->Rotate(vec3(rot));

		// Prevent large jumps. Note: audio should be updated before doing this.
		if (delta > max_delta) {
			Log->Print("Frame took too long; time has been limited.");
			delta = max_delta;
		}
		then = now;

		// Update input before logic/objects so that it's not running behind.
		Input->Update();
		Scene->Update(delta);
		Game->Render();
	}

	Audio->Close();

	delete Scene;
	delete Lua;
	delete Game;
	delete Audio;
	delete Input;
	delete Resources;
	delete File;
	delete Log;

	glfwTerminate();

	return 0;
}
