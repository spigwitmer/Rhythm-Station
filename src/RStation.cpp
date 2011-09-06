#include <GL3/gl3w.h>
#include <glsw.h>
#include "RStation.h"
#include "managers/DisplayManager.h"
#include "managers/InputManager.h"
#include "managers/LuaManager.h"
#include "managers/ScreenManager.h"
#include "utils/Logger.h"
#include <deque>
#include <glm/glm.hpp>

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

inline void endian_swap(unsigned short &x)
{
    x = (x>>8) | 
	(x<<8);
}

inline void endian_swap(unsigned int &x)
{
    x = (x>>24) | 
	((x<<8) & 0x00FF0000) |
	((x>>8) & 0x0000FF00) |
	(x<<24);
}

// __int64 for MSVC, "long long" for gcc
inline void endian_swap(unsigned long long &x)
{
    x = (x>>56) | 
	((x<<40) & 0x00FF000000000000) |
	((x<<24) & 0x0000FF0000000000) |
	((x<<8)  & 0x000000FF00000000) |
	((x>>8)  & 0x00000000FF000000) |
	((x>>24) & 0x0000000000FF0000) |
	((x>>40) & 0x000000000000FF00) |
	(x<<56);
}

int RStation::Run()
{
	FileManager fileman;
	InputManager input;
	ScreenManager screen;
	DisplayManager display;
	
	// Open the display, make sure nothing went wrong on init.
	if (!display.OpenWindow())
		return 1;
	
	/*
	// Input device drivers (lua based)
	LuaManager drivers(fileman);
	drivers.Bind("/devices/");
	
	// Game scripts.
	LuaManager game(fileman);
	game.Bind("/screens/");
	*/
	
	long last_update = 0;
	double then = glfwGetTime(), now = 0.0;
	std::deque<double> times;
	while (true)
	{
		double delta = 0.0;
		now = glfwGetTime();
		delta = now - then;

		// TODO: Move to ScreenManager
		{
			// Keep 5 seconds worth of deltas (@60fps)
			const size_t NUM_FRAMES = 60*5;
			double avg = 0.0;
			
			// Calculate Average FPS.
			times.push_back(delta);
			
			if (times.size() > NUM_FRAMES)
				times.pop_front();
			
			for (std::deque<double>::iterator i = times.begin(); i != times.end(); i++)
				avg += *i;
			
			if (int(now) % 1 == 0 && avg > 0.0001)
			{
				if (last_update != int(now))
					LOG->Info("Avg. FPS: %0.0f",
					  glm::ceil(1.0 / (avg / times.size())));
				last_update = int(now);
			}
		}
		
		then = now;
		
		// Break if user closed the window
		input.Update();
		
		if (!glfwIsWindow(display.GetWindow()) || input.GetButton(RS_KEY_ESC)->IsDown())
			break;
		
		screen.Update(delta);
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
