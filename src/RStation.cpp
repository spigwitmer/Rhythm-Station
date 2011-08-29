#include "RStation.h"
#include "managers/InputManager.h"
#include "managers/ScreenManager.h"
#include "managers/DisplayManager.h"
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
	DisplayManager display;
	
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
 * @file
 * @author Colby Klein (c) 2011
 * @section LICENSE
 * 
 * This program is licensed under the terms of the MIT license.
 * The full text can be found in License.txt.
 */
