#include "RStation.h"
#include "managers/DisplayManager.h"
#include "managers/LuaManager.h"
#include "managers/ScreenManager.h"
#include "managers/InputManager.h"

// These two are singletons for convenience.
#include "managers/MessageManager.h"
#include "utils/Logger.h"

#include "global.h"

using namespace std;

RStation::RStation(vector<string> &vArgs)
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
	ScreenManager screen(lua, fileman);
	
	// Open the display, make sure nothing went wrong on init.
	if (!display.OpenWindow())
		return 1;
	
	lua.Init("Data/Scripts");
	screen.PushScreen("ScreenTest");
	
	while (true)
	{
		// Break if user closed the window
		input.Update();
		
		if (unlikely(!glfwIsWindow(display.GetWindow())))
			break;
		
		if (unlikely(input.GetButton(RS_KEY_ESC)->IsDown()))
			break;
		
		screen.Update();
		screen.Draw();
		
		if (unlikely(!display.IsFocused()))
			usleep(50000);
		
		display.CheckError();
		display.Flush();
	}
	
	// Don't bother broadcasting messages after the window closes.
	MessageManager::GetSingleton()->Clear();
	
	display.CloseWindow();
	
	return 0;
}

int main(int argc, char **argv)
{
	// Build arguments into a vector before passing control
	vector<string> args;
	for (int i = 0; i<argc; i++)
		args.push_back(string(argv[i]));
	
	RStation rs(args);
	return rs.Run();
}

/**
 * Colby Klein (c) 2011
 * Licensed under the terms of the MIT license. See License.txt.
 */
