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
#include "utils/Logger.h"
#include "Window.h"
#include "PreferencesFile.h"
#include "threads/ThreadGroup.h"

#if 0
void *MyWin32ThreadTest(void *data)
{
	Log->Print("Hello from child thread!");
	SleepEx(3000, false);

	return NULL;
}
#endif

// Initialize everything and set up the GL states used throughout the program.
int main (int argc, char **argv)
{
	// Startup Logger singleton first
	Log = new Logger();

#if 0
	Log->Print("RStation win32 thread test");
	RSThread mythread;
	mythread.Start(MyWin32ThreadTest);
	mythread.Wait();
	
	delete Log;	// XXX
	exit(0);	// XXX
#endif
	
	// Load Preferences
	Preferences = new PreferencesFile("Preferences.ini");
	
	if (!glfwInit()) {
		Log->Print("Something is terribly wrong!");
		return 0;
	}
	
	Window *wnd = Window::getSingleton();
	
	wnd->Create(
		Preferences->GetLongValue("Graphics", "WindowWidth", 854),
		Preferences->GetLongValue("Graphics", "WindowHeight", 480),
		Preferences->GetBoolValue("Graphics", "FullScreen", false));
	glfwSwapInterval(Preferences->GetBoolValue("Graphics", "VSync", false));
	
	// Start up all our singletons.
	Game = new GameManager(wnd->getWindow());
	
	// These ones should clean themselves up.
	InputManager *Input = InputManager::getSingleton();
	LuaManager *Lua = LuaManager::getSingleton();
	
	// Connect event callbacks
	Input->Connect();
	wnd->Connect();
	
	// Handle the arguments before doing anything else
	HandleArguments(argc, argv);
	
	SoundManager::Open();
	
	// Run the theme's lua scripts.
	Lua->Run();
	
	// we'll be using these everywhere, enable them and leave it that way.
	glEnableVertexAttribArray(VERTEX_ARRAY);
	glEnableVertexAttribArray(NORMAL_ARRAY);
	glEnableVertexAttribArray(COORD_ARRAY);
	
	double then = glfwGetTime(); // prevent registering a skip on first update
	double max_delta = (1.0/60.0) * 3.0;
	double now = then;
	
	while (glfwIsWindow(wnd->getWindow()))
	{
		if (glfwGetKey(wnd->getWindow(), GLFW_KEY_ESC))
			break;
		
		if (now < then)
			now += fabs(now - then);
		else
			now = glfwGetTime();
		
		double delta = fabs(now - then);
		
		/*
		 * Check that the window is active and update the title twice per second.
		 * Do this before limiting the delta so it always reports the true value.
		 */
		if (int(then * 2) != int(now * 2))
			wnd->UpdateTitle(delta);
		
		// Prevent large jumps. Note: audio should be updated before doing this.
		if (delta > max_delta)
		{
			Log->Print("Frame took too long; time has been limited.");
			delta = max_delta;
		}
		
		then = now;
		
		Game->Update(delta);
		Input->Update(); // objects will update on the message
		Game->Render();
	}
	
	SoundManager::Close();
	
	delete Game;
	delete Log;
	delete Preferences;
	
	glfwTerminate();
	
	return 0;
}


/**
 * @file
 * @author Colby Klein (c) 2011
 * @section LICENSE
 * 
 * All rights reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, and/or sell copies of the Software, and to permit persons to
 * whom the Software is furnished to do so, provided that the above
 * copyright notice(s) and this permission notice appear in all copies of
 * the Software and that both the above copyright notice(s) and this
 * permission notice appear in supporting documentation.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF
 * THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS
 * INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
