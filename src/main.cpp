// GL headers
#include <GL/glew.h>
#include <GL/glfw3.h>

// all the singletons (we init them here, should do static init?)
#include "AudioManager.h"
#include "FileManager.h"
#include "GameManager.h"
#include "InputManager.h"
#include "LuaManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Logger.h"

// misc
#include "HandleArguments.h"
#include "Object.h"

Matrix *g_projection_matrix = NULL;
GLFWwindow window = 0;

namespace Util {
	void UpdateWindowTitle(double delta) {
		using namespace std;

		double fps = 1.f / delta;

		ostringstream str;
		str << "Rhythm Station - ";
		str << "FPS: " << int(fps * 10) * 0.1f << ", ";
		str << "Delta: " << delta;

		string sfps = str.str(); // str.c_str doesn't work?

		glfwSetWindowTitle(window, sfps.c_str());
	}
}

int main (int argc, char** argv) {
	glfwInit();

	GLFWvidmode mode;
	glfwGetDesktopMode(&mode);
	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4); // 4x MSAA
	glfwOpenWindowHint(GLFW_DEPTH_BITS, 32);
	window = glfwOpenWindow(854, 480, GLFW_WINDOWED, "", 0);
	glfwSwapInterval(1);
	glewInit();

	// start up all our singletons.
	Log		= new Logger();
	File		= new FileManager();
	Resources	= new ResourceManager();
	Game	= new GameManager(window);
	Scene	= new SceneManager();
	Audio	= new AudioManager();
	Input	= new InputManager();
	Lua		= new LuaManager();

	// handle the arguments before doing anything else
	HandleArguments(argc, argv);

	Audio->Open();

	// generate the vbo for quads
	GenerateQuadBuffers();

	// Is this available on windows only? It isn't on OS X, at least.
	if (GLEW_NV_framebuffer_multisample_coverage)
		Log->Print("CSAA Supported.");
		// 8x = 8, 4; 8xQ = 8, 8; 16x = 16, 4; 16xQ = 16, 8
		// glRenderbufferStorageMultisampleCoverageNV(buffer, 8, 4, GL_RGB, 0, 0);
	else
		Log->Print("CSAA Not Supported.");

	// Start running Lua and begin the first screen.
	Game->Start();

	double then = 0.0;
	double max_delta = (1.0/60.0) * 3.0;
	while (glfwIsWindow(window)) {
		double now = glfwGetTime();
		double delta = now - then;

		// prevent large jumps.
		if (delta > max_delta) {
			Log->Print("Frame took too long; time has been limited.");
			delta = max_delta;
		}

		// update window title 4 times per second.
		if (int(then * 4) != int(now * 4))
			Util::UpdateWindowTitle(delta);

		then = now;

		/*
		 * Update the input immediately before drawing the scene so that
		 * we're on the correct frame and not one behind.
		 */
		Input->Update();

		// Game should be changed to Scene.
		Game->Update(delta);
		Game->Render();
	}

	Audio->Close();

	DeleteQuadBuffers();

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
