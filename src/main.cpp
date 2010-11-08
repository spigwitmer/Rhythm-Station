// GL headers
#include <GL/glew.h>
#include <GL/glfw3.h>

#include "globals.h"

// All the singletons (we init them here, should do static init?)
#include "AudioManager.h"
#include "FileManager.h"
#include "GameManager.h"
#include "InputManager.h"
#include "LuaManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Logger.h"

// Misc
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

vec2 g_res;

#ifndef DEBUG
#ifdef _WIN32
// TODO: Different main.
#endif
#endif
// Initialize everything and set up the GL states used throughout the program.
int main (int argc, char** argv) {
	glfwInit();

	GLFWvidmode mode;
	glfwGetDesktopMode(&mode);
	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4); // 4x MSAA
	glfwOpenWindowHint(GLFW_DEPTH_BITS, 32);
	window = glfwOpenWindow(854, 480, GLFW_WINDOWED, "", 0);
	glfwSwapInterval(0);
	glewInit();

	int width, height;
	glfwGetWindowSize(window, &width, &height);
	g_res = vec2(width, height);

	// Make transparency work!
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Start up all our singletons.
	Log		= new Logger();
	File		= new FileManager();
	Resources	= new ResourceManager();
	Game	= new GameManager(window);
	Scene	= new SceneManager();
	Audio	= new AudioManager();
	Input	= new InputManager();
	Lua		= new LuaManager();

	// Handle the arguments before doing anything else
	HandleArguments(argc, argv);

	Audio->Open();

	// Generate the VBO used by quads.
	GenerateQuadBuffers();

	std::ostringstream extensions;
	extensions << "Available OpenGL extensions: \n";
	extensions << glGetString(GL_EXTENSIONS);
	Log->Print(extensions.str());

	// move this elsewhere
	// Is this only available on windows?
	if (GLEW_NV_framebuffer_multisample_coverage)
		Log->Print("CSAA Supported.");
		// 8x = 8, 4; 8xQ = 8, 8; 16x = 16, 4; 16xQ = 16, 8 (this will be elsewhere)
		// glRenderbufferStorageMultisampleCoverageNV(buffer, 8, 4, GL_RGB, 0, 0);
	else
		Log->Print("CSAA Not Supported.");

	// Start running Lua and begin the first screen.
	Game->Start();
	Lua->Start();

	double then = 0.0;
	double max_delta = (1.0/60.0) * 3.0;
	while (glfwIsWindow(window)) {
		double now = glfwGetTime();
		double delta = fabs(now - then);

		/*
		 * Check that the window is active and update the title twice per second.
		 * Do this before limiting the delta so it always reports the true value.
		 */
		if (int(then * 2) != int(now * 2)) {
			Util::UpdateWindowTitle(delta);
			Game->SetActive(glfwGetWindowParam(Game->GetWindow(), GLFW_ACTIVE));
		}

		// Prevent large jumps. Note: audio should be updated before doing this.
		if (delta > max_delta) {
			Log->Print("Frame took too long; time has been limited.");
			delta = max_delta;
		}

		then = now;

		// Update input before logic/objects so that it's not running behind.
		Input->Update();
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
