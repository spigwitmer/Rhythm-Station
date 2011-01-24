// GL headers
#include <GL/glew.h>
#include <GL/glfw3.h>
#include "HandleArguments.h"

// All the singletons (we init them here, should do static init?)
#include "SoundManager.h"
#include "GameManager.h"
#include "InputManager.h"
#include "LuaManager.h"
#include "ResourceManager.h"
#include "Logger.h"
#include "Window.h"
#include "PreferencesFile.h"

// Initialize everything and set up the GL states used throughout the program.
int main (int argc, char **argv)
{
	// Startup Logger singleton first
	Log = new Logger();
	
	// Load Preferences
	Preferences = new PreferencesFile("Preferences.ini");
	
	glfwInit();
	Window::Create(
		Preferences->GetLongValue("Graphics", "WindowWidth", 854),
		Preferences->GetLongValue("Graphics", "WindowHeight", 480),
		Preferences->GetBoolValue("Graphics", "FullScreen", false));
	glfwSwapInterval(Preferences->GetBoolValue("Graphics", "VSync", true));
	glEnable(GL_DEPTH_TEST);
	
	// Start up all our singletons.
	Game	= new GameManager(Window::GetWindow());
	Input	= new InputManager();
	Lua		= new LuaManager();
	
	// Connect event callbacks
	Input->Connect();
	
	// Handle the arguments before doing anything else
	HandleArguments(argc, argv);
	
	SoundManager::Open();
	
	// Start running Lua and begin the first screen.
	Game->Start();
	Lua->Run("init.lua");
	
	// we'll be using these everywhere, enable them and leave it that way.
	glEnableVertexAttribArray(VERTEX_ARRAY);
	glEnableVertexAttribArray(NORMAL_ARRAY);
	glEnableVertexAttribArray(COORD_ARRAY);
	
	double then = glfwGetTime(); // prevent registering a skip on first update
	double max_delta = (1.0/60.0) * 3.0;
	
	while (glfwIsWindow(Window::GetWindow()))
	{
		if (glfwGetKey(Window::GetWindow(), GLFW_KEY_ESC))
			break;

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
	
	SoundManager::Close();
	
	delete Lua;
	delete Game;
	delete Input;
	delete Log;
	delete Preferences;
	
	glfwTerminate();
	
	return 0;
}
