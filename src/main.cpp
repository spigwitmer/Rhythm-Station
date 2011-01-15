// GL headers
#include <GL/glew.h>
#include <GL/glfw3.h>
#include "HandleArguments.h"

// All the singletons (we init them here, should do static init?)
#include "AudioManager.h"
#include "GameManager.h"
#include "InputManager.h"
#include "LuaManager.h"
#include "ResourceManager.h"
#include "Logger.h"
#include "Window.h"

#include "ThreadGroup.h"

#include "Mesh.h"

// temp
void *print_stuff(void *arg)
{
	ThreadParameters* t = (ThreadParameters*)arg;

	// do stuff

	// lock for whatever reason
	if(t->group->Lock() == 0)
	{
		// sync some data
		*((int *)(t->argument)) = *((int *)(t->argument)) + 1;
		// and unlock again
		t->group->Unlock();
	}

	return NULL;
}

void test_threads()
{
	int n_groups = 10, n_threads = 10, sum[n_groups];
	ThreadGroup threadgroups[n_groups];
	for (int g = 0; g<n_groups; g++)
	{
		sum[g] = 0;
		for (int t = 1; t<=n_threads; t++)
			threadgroups[g].CreateThread(&print_stuff,&sum[g]);
		threadgroups[g].JoinAll();
		Log->Print("Threadtest sum[%i] = %i",g,sum[g]);
	}
}

// Initialize everything and set up the GL states used throughout the program.
int main (int argc, char** argv)
{
	glfwInit();

	Window::Create(854, 480);
	glfwSwapInterval(1);
	glEnable(GL_DEPTH_TEST);

	// Start up all our singletons.
	Log			= new Logger();
	Game		= new GameManager(Window::GetWindow());
	Input		= new InputManager();
	Lua			= new LuaManager();

	// Connect event callbacks
	Input->Connect();

	// Handle the arguments before doing anything else
	HandleArguments(argc, argv);

	AudioManager::Open();

	// Start running Lua and begin the first screen.
	Game->Start();
	Lua->Start();

	// we'll be using these everywhere, enable them and leave it that way.
	glEnableVertexAttribArray(VERTEX_ARRAY);
	glEnableVertexAttribArray(NORMAL_ARRAY);
	glEnableVertexAttribArray(COORD_ARRAY);

	// Testing.
	float vertices[] =
	{
		 1, -1, -1, 0, 0, 1, 0, 0,
		 1, -1,  1, 0, 1, 0, 0, 0,
		-1, -1,  1, 1, 1, 0, 0, 0,
		-1, -1, -1, 0, 0, 1, 0, 0,
		 1,  1, -1, 0, 1, 0, 0, 0,
		 1,  1,  1, 1, 0, 0, 0, 0,
		-1,  1,  1, 0, 0, 1, 0, 0,
		-1,  1, -1, 0, 1, 0, 0, 0
	};

	unsigned indices[] = {
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

	MeshData verts[8];
	memcpy(&verts[0].Position.x, vertices, sizeof(float) * 8 * 8);	
	
	Mesh cube;
	cube.Load(verts, indices, 8, 3*12);

	test_threads();

	double then = glfwGetTime(); // prevent registering a skip on first update
	double max_delta = (1.0/60.0) * 3.0;
	while (glfwIsWindow(Window::GetWindow()))
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

		// Prevent large jumps. Note: audio should be updated before doing this.
		if (delta > max_delta) {
			Log->Print("Frame took too long; time has been limited.");
			delta = max_delta;
		}
		then = now;

		Game->Update(delta);

		Input->Update(); // objects will update on the message
		Game->Render();
	}

	AudioManager::Close();

	delete Lua;
	delete Game;
	delete Input;
	delete Log;

	glfwTerminate();

	return 0;
}
