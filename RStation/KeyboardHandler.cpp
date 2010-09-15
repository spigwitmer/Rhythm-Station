#include "KeyboardHandler.h"
#include "SceneManager.h"
#include "Log.h"

KeyStates Keys[GLFW_KEY_LAST] = { RS_FIRST_PRESS };

void KeyCallback(int key, int action)
{
	switch ( action )
	{
	case GLFW_PRESS:
		if ( Keys[key] == RS_FIRST_PRESS )
			Keys[key] = RS_HELD;
		else
			Keys[key] = RS_FIRST_PRESS;
		break;
	case GLFW_RELEASE:
		if ( Keys[key] == RS_HELD || Keys[key] == RS_FIRST_PRESS )
			Keys[key] = RS_LET_GO;
		else
			Keys[key] = RS_NONE;
		break;
	}

	IEvent event;
	event.Key = (KeyAliases)key;
	event.State = Keys[key];

	// flush log on F3
	if(event.Key == KEY_F3 && event.State == RS_FIRST_PRESS)
	{
		Log::Write();
	}

	Scene::SendInput(event);
}

void RegisterKeyboardCallbacks()
{
	glfwSetKeyCallback(KeyCallback);
	
	// this is more relevant for keyboard than mouse.
	glfwDisable(GLFW_AUTO_POLL_EVENTS);
	
	Log::Print("Registered KeyboardHandler.");
}
