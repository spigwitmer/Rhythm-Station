#include <GL/glew.h>

// Oh you.
#if __APPLE__
// Throws a warning because it has to unless I pull off some needless voodoo.
#include <OpenGL/gl3.h>
#endif

#include "RStation.h"
#include "managers/InputManager.h"
#include "managers/ScreenManager.h"
#include "renderer/Display.h"
#include "utils/Logger.h"

using namespace std;

class FileManager
{
public:
	FileManager() {}
};

class LuaManager
{
public:
	LuaManager(FileManager f) {}
	void Init() {}
	void Bind(string s) {}
};

RStation::RStation(std::vector<std::string> &vArgs)
{
	m_vArgs = vArgs;

	LOG = new Logger();
	
	int err = glfwInit();
	if (!err)
		LOG->Fatal(string(glfwErrorString(err)));
}

RStation::~RStation()
{
	SAFE_DELETE(LOG);
}

int RStation::Run()
{
	FileManager fileman;
	InputManager input;
	ScreenManager screen;
	
	// Shared window params, we always want these.
	glfwOpenWindowHint(GLFW_DEPTH_BITS, 24);
	glfwOpenWindowHint(GLFW_STENCIL_BITS, 8);
	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 0);
	glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, 1);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// First, try to create a GL 3.2 context
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
	m_window = glfwOpenWindow(854, 480, GLFW_WINDOWED, "Rhythm-Station (GL 3.2)", NULL);
	
	// if we were able to create a window, then we support GL 3.2
	bool bUsingGL3 = glfwIsWindow(m_window) ? true : false;
	
	// We weren't able to make a GL 3.2 window. Falling back to GL 2.1.
	if (!bUsingGL3) {
		LOG->Warn("Unable to create an OpenGL 3.2 context. Falling back to 2.1");
		glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 2);
		glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 1);
		m_window = glfwOpenWindow(854, 480, GLFW_WINDOWED, "Rhythm-Station (GL 2.1)", NULL);
	}
	
	// Make sure we were able to create a rendering context.
	if (!glfwIsWindow(m_window))
	{
		LOG->Warn("Unable to create an OpenGL window. Check your drivers.");
		return 2;
	}

	// We only need GLEW for legacy contexts.
	if (!bUsingGL3)
		glewInit();

	Display::Init(bUsingGL3);
	Display::CheckError();
	
	// Input device drivers (lua based)
	LuaManager drivers(fileman);
	drivers.Bind("/devices/");
	
	// Game scripts.
	LuaManager game(fileman);
	game.Bind("/screens/");
	
	while (true)
	{
		// Break if user closed the window
		input.Update();
		if (!glfwIsWindow(m_window) || input.GetButton(RS_KEY_ESC)->IsDown())
			break;
		
		// ScreenManager automatically calculates delta.
		screen.Update(glfwGetTime());
		screen.Draw();
		
		glfwSwapBuffers();
	}
	
	return 0;
}

/**
 * @file
 * @author Colby Klein (c) 2011
 * @section LICENSE
 * 
 * This program is licensed under the terms of the MIT license.
 * The full text can be found in License.txt.
 */
