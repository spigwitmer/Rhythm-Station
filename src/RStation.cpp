#include "RStation.h"
#include "managers/DisplayManager.h"
#include "managers/InputManager.h"
#include "managers/LuaManager.h"
#include "managers/ScreenManager.h"
#include "utils/Logger.h"

using namespace std;

RStation::RStation(std::vector<std::string> &vArgs)
{
	int err = glfwInit();
	m_vArgs = vArgs;
	LOG = new Logger();
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
	DisplayManager display;
	
	LuaManager lua(fileman);
	ScreenManager screen(lua);
	
	lua.Init("Data/Scripts");
	screen.PushScreen("ScreenTest");
	
	// Open the display, make sure nothing went wrong on init.
	if (!display.OpenWindow())
		return 1;
	
	while (true)
	{
		// Break if user closed the window
		input.Update();
		
		if (!glfwIsWindow(display.GetWindow()) || input.GetButton(RS_KEY_ESC)->IsDown())
			break;
		
		screen.Update();
		screen.Draw();
		
		if (!display.IsFocused())
			usleep(50000);
		
		display.CheckError();
		display.Flush();
	}
	
	display.CloseWindow();
		
	return 0;
}

/**
 * Colby Klein (c) 2011
 * Licensed under the terms of the MIT license. See License.txt.
 */
