#include "RStation.h"
#include <GL/glfw.h>
#include "GameLoop.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Screen.h"
#include <sstream>

bool bRunning = true;

// this will need to be declared in SceneManager probably.
InputManager* Input = new InputManager();

const int freq = 2; // update x times per second

namespace Game
{
	void Terminate()
	{
		bRunning = false;
	}
	void Run()
	{
		Scene::PushScreen(); // push initial screen
		
		double then = glfwGetTime();
		while(bRunning && glfwGetWindowParam(GLFW_OPENED))
		{
			// calculate delta time
			double now = glfwGetTime();
			float delta = float(now - then);
			
			// calculate FPS and set window title.
			if( int(then * freq) != int(now * freq) )
			{
				double fps = 1.f / delta;
				
				std::ostringstream str;
				str << "RStation - ";
				str << "FPS: ";
				str << int(fps * 10) * 0.1f;
				str << ", Delta: ";
				str << delta;
				
				std::string sfps = str.str(); // str.c_str doesn't work?
				glfwSetWindowTitle(sfps.c_str());
			}
			then = now;
			
			Scene::Update(delta);			
			Input->Update(); // singleton for some reason
			
			// Draw!
			Scene::Draw();
		}
		
		Scene::Clear();
	}
}
