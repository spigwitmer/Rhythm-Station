#include "RStation.h"
#include "managers/DisplayManager.h"
#include "managers/InputManager.h"
#include "managers/LuaManager.h"
#include "managers/ScreenManager.h"
#include "utils/Logger.h"
#include <glsw.h>

using namespace std;

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
	DisplayManager display;
	
	char buf[1024];
	getcwd(buf, 1024);
	
	const char* shader = glswGetShader("Something.Vertex.GL32");
	if (shader == NULL) 
		LOG->Info("(%s) %s", buf, glswGetError());
	else
		LOG->Info("%s", shader);
	
	fileman.Mount("/", string(buf));
	
	// Open the display, make sure nothing went wrong on init.
	display.OpenWindow(m_window);
	display.CheckError();
	
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
		
		display.Flush();
	}
	
	return 0;
}

/**
 * Colby Klein (c) 2011
 * Licensed under the terms of the MIT license. See License.txt.
 */
