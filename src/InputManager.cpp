#include <GL/glfw3.h>
#include <stdio.h>
#include "InputManager.h"
#include "GameManager.h"
#include "LuaManager.h"
#include "INIFile.h"
#include "Logger.h"
#include "PreferencesFile.h"
#include "Window.h"

InputManager *Input = NULL;

// keyboard. key for specials, char for text input and such.
static void _keyCallback(GLFWwindow window, int key, int state)
{
	double cur_time = glfwGetTime();
	
	switch (state) {
	case GLFW_PRESS:
		Input->status.keys[key] = KEY_PRESSED;
		
		switch (key)
		{
		case 294:
			Log->Print(Input->status.cur_string);
			Input->status.cur_string.clear();
			break;
		case 295: // backspace
			Input->status.cur_string = Input->status.cur_string.substr(0, Input->status.cur_string.length()-1);
		default:
			break;
		}
		
		break;
	case GLFW_RELEASE:
		Input->status.keys[key] = KEY_NONE;
		break;
	}
	
	Input->status.timestamp[key] = cur_time;
	Input->SendEvent();
}

static void _charCallback(GLFWwindow window, int key)
{
	char buf[2];
	sprintf(buf, "%c", key);
	Input->status.cur_string.push_back(buf[0]);
}

// mouse actions
static void _mPosCallback(GLFWwindow window, int x, int y)
{
	// Log->Print("x = %d, y = %d", x, y);
	Input->status.mouse.x = x;
	Input->status.mouse.y = y;
	Input->status.mouse.nx = float(x / Window::getWidth());
	Input->status.mouse.ny = float(y / Window::getHeight());
	Input->SendEvent();
}

static void _mButtonCallback(GLFWwindow window, int button, int state)
{
	Input->status.mouse.buttons[button] = state == GLFW_PRESS ? KEY_PRESSED : KEY_NONE;
	Input->SendEvent();
}

static void _mScrollCallback(GLFWwindow window, int x, int y)
{
	Input->status.mouse.scroll = vec2(x, y);
	Input->SendEvent();
}

InputManager::InputManager()
{
	DetectControllers();
	status.keys = new KeyState[GLFW_KEY_LAST];
	
	for (unsigned i = 0; i < GLFW_KEY_LAST; ++i)
		status.keys[i] = KEY_NONE;
		
	status.timestamp = new double[GLFW_KEY_LAST];
	status.mouse.buttons = new KeyState[GLFW_MOUSE_BUTTON_LAST];
	queuedUpdate = false;
}

void InputManager::Connect()
{
	glfwSetKeyCallback(_keyCallback);
	glfwSetCharCallback(_charCallback);
	glfwSetMousePosCallback(_mPosCallback);
	glfwSetMouseButtonCallback(_mButtonCallback);
	glfwSetScrollCallback(_mScrollCallback);
}

InputManager::~InputManager()
{
	glfwSetKeyCallback(NULL);
	glfwSetCharCallback(NULL);
	glfwSetMousePosCallback(NULL);
	glfwSetMouseButtonCallback(NULL);
	glfwSetScrollCallback(NULL);
	
	for (unsigned int i = 0; i<status.controllers.size(); i++)
		delete status.controllers[i];
	
	status.controllers.clear();
	delete[] status.mouse.buttons;
	delete[] status.keys;
	delete[] status.timestamp;
}

Controller::Controller(int _id)
{
	this->id = _id;
	this->num_axes = glfwGetJoystickParam(this->id, GLFW_AXES);
	this->num_buttons = glfwGetJoystickParam(this->id, GLFW_BUTTONS);
	// I'm not entirely sure this is required, but it's probably not a bad idea.
	this->axes = new float[this->num_axes];
	this->buttons = new KeyState[this->num_buttons];
	this->buttons_raw = new unsigned char[this->num_buttons];
	this->timestamp = new double[this->num_buttons];
	
	for (int i = 0; i<this->num_buttons; i++)
	{
		this->buttons_raw[i] = 0;
		this->buttons[i] = KEY_NONE;
	}
	
	// initial update.
	glfwGetJoystickButtons(this->id, this->buttons_raw, this->num_buttons);
	glfwGetJoystickPos(this->id, this->axes, this->num_axes);
	Log->Print("Creating controller");
}

Controller::~Controller()
{
	Log->Print("Destroying controller %d", this->id+1);
	delete[] this->axes;
	delete[] this->buttons;
	delete[] this->buttons_raw;
}

void InputManager::SendEvent()
{
	Message msg;
	msg.name = "Input";
//	msg.data["Event"] = (void*)status;
	msg.Send();
}

void InputManager::DetectControllers()
{
	// find all our joysticks
	for (int i = 0; i<GLFW_JOYSTICK_LAST; i++)
	{
		int present = glfwGetJoystickParam(i, GLFW_PRESENT);
		
		if (present) {
			// joystick/controller was found - register it and get info.
			Controller *current = new Controller(i);
			status.controllers.push_back(current);
		}
	}
	
	// log what we found
	if (status.controllers.size() > 0)
	{
		Log->Print("Controllers found:");
		
		for (unsigned int i = 0; i<status.controllers.size(); i++)
		{
			Log->InlinePrint(
			    "\tController %d (id=%d, buttons=%d, axes=%d)\n", i+1,
			    status.controllers[i]->id,
			    status.controllers[i]->num_buttons,
			    status.controllers[i]->num_axes
			);
		}
	}
}

void InputManager::UpdateControllers()
{
	// update all joystick buttons + axes.
	for (unsigned int i = 0; i<status.controllers.size(); i++)
	{
		Controller *current = status.controllers[i];
		
		// store old values for comparison
		unsigned char *old_buttons = new unsigned char[current->num_buttons]; // FIXME
		float *old_axes = new float[current->num_axes]; // FIXME
		memcpy(old_buttons, current->buttons_raw, sizeof(old_buttons));
		memcpy(old_axes, current->axes, sizeof(old_axes));
		
		// update the current values
		glfwGetJoystickButtons(current->id, current->buttons_raw, current->num_buttons);
		glfwGetJoystickPos(current->id, current->axes, current->num_axes);
		bool sendInput = false;
		
		// and now set timestamp if a button has changed.
		for (int j = 0; j<current->num_buttons; j++)
		{
			if (old_buttons[j] != current->buttons_raw[j] || queuedUpdate)
			{
				double cur_time = glfwGetTime();
				
				switch(current->buttons_raw[j])
				{
				case GLFW_PRESS:
					current->buttons[j] = KEY_PRESSED;
					break;
				case GLFW_RELEASE:
					current->buttons[j] = KEY_NONE;
					break;
				}
				
				current->timestamp[j] = cur_time;
				sendInput = true;
			}
		}
		
		for (int j = 0; j<current->num_axes; j++)
		{
			if (old_axes[j] != current->axes[j] && fabs(current->axes[j]) > 7.5e-2)
				sendInput = true;
		}
		
		delete [] old_buttons;
		delete [] old_axes;
		
		if (sendInput)
			SendEvent();
	}
}

void InputManager::Update()
{
	glfwPollEvents();
	
	// controllers aren't driven by a callback - so we have to do it here.
	// this *REALLY* should be threaded at some point.
	UpdateControllers();
}
