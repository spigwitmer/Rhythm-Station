#include <GL/glew.h>
#include <GL/glfw.h>
#include "RStation.h"
#include "GameLoop.h"
#include "KeyboardHandler.h"
#include "MouseHandler.h"
#include "AudioManager.h"
#include "INILoader.h"
#include "LuaManager.h"
#include "FileManager.h"
#include "SceneManager.h"
#include <cstring>
#ifdef __APPLE__
#include "dialog.h"
#endif

INILoader* ini = new INILoader();

bool g_mouse_detached = false;

void GLFWCALL ResizeViewport(int w, int h)
{
	// half width, half height.
	int hw = int(w*0.5f);
	int hh = int(h*0.5f);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, w, h);

	glOrtho(-hw, hw, hh, -hh, -hw, hw);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// (re)register mouse callback.
	RegisterMouseCallbacks();
}

void SetInitialStates()
{
	// the frame buffer is usually full of junk after resize. clear it.
	glClearColor(0.0f, 0.0f, 0.0f, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D); // not needed when shaders are working
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void InitWindow(int _width, int _height)
{
	if (!glfwOpenWindow(_width, _height, /* rgba */ 0,0,0,8, /* depth, stencil, mode */ 24,1, GLFW_WINDOW))
	{
		Log->Print("Window creation failed!");
		glfwTerminate();
		exit(-1);
	}
	glewInit();

	// The window title will be overridden less than a second from startup anyways.
	glfwSetWindowTitle("RStation");

	// todo: preference namespace/class (whichever is more appropriate)
	int vsync = atoi(ini->getValue("Preferences","VSync","1").c_str());
	vsync = vsync ? 1 : 0;
	std::string vs_str;
	if (vsync)
		vs_str = "enabled";
	else
		vs_str = "disabled";

	Log->Print("VSync: " + vs_str);
	glfwSwapInterval(vsync); // vsync

	glfwSetWindowSizeCallback(ResizeViewport);
	RegisterKeyboardCallbacks();
}

float g_aspectRatio = 0.0f;

// TODO: handle command line args for windows and unix
#ifdef _WIN32
#define argc __argc
#define argv __argv
int WINAPI WinMain(HINSTANCE d1, HINSTANCE d2, LPSTR d3, int d4)
#else
int main(int argc, char** argv)
#endif
{
	glfwInit();

	// initialize everything needed
	Log = new Logger();
	File = new FileManager();
	Scene = new SceneManager();
	Audio = new AudioManager();

	for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i],"--detach-cursor") == 0)
		{
			Log->Print("Disabling cursor.");
			g_mouse_detached = true;
		}
	}

	ini->Load("GameData/Preferences.ini");

	// get the resolution from prefs
	int width = atoi(ini->getValue("Preferences","DisplayWidth","854").c_str());
	int height = atoi(ini->getValue("Preferences","DisplayHeight","480").c_str());
	g_aspectRatio = width / height;

	InitWindow(width, height);

	// we're done reading the ini, nuke it.
	delete ini;

	/*
	 * Shaders are used for a lot here, so of course OpenGL 2.0 is required.
	 * In the future I may start using Cg, although I like GLSL more it would allow
	 * having a DX renderer without a ton of dupe shaders.
	 */
	if (!GLEW_VERSION_2_0)
	{
		Log->Print("OpenGL 2.0 is not supported. You may need to update your drivers.");

		// Dialog needed on other platforms!
#ifdef __APPLE__
		Alert(); // exits if not ignored
#else
		return 1;
#endif
	}

	lua_testLoad();

	// Set up our defaults and pass control.
	SetInitialStates();
	Game::Run();

	glfwTerminate();

	delete Audio;
	delete Log;
	return 0;
}
