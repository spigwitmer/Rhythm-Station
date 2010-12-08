#include <GL/glfw3.h>
#include <stdio.h>
#include "InputManager.h"
#include "GameManager.h"
#include "LuaManager.h"
#include "Logger.h"
#include "globals.h"

InputManager* Input = NULL;

std::string str;

// keyboard. key for specials, char for text input and such.
void keyCallback(GLFWwindow window, int key, int state) {
	if (state == GLFW_PRESS) {
		// testing
		if (key == 295) {
			str = str.substr(0,str.length()-1);
			return;
		}
		if (key == 294) {
			Log->Print(str);
			str.clear();
		}
		Log->Print("key: %d\n", key);
	}
}

void charCallback(GLFWwindow window, int key) {
	char buf[2];
	sprintf(buf, "%c", key);
	str.push_back(buf[0]);
}

// mouse actions
void mPosCallback(GLFWwindow window, int x, int y) {
	// todo
}

void mButtonCallback(GLFWwindow window, int button, int state) {
	// todo
}

void mScrollCallback(GLFWwindow window, int x, int y) {
	// todo
	// + check args on this function.
}

// on window resize
void resizeCallback(GLFWwindow window, int width, int height) {
	g_projection_matrix->Ortho(width, height, vec2(-100, 100));
	Game->QueueRendering();
}

InputManager::InputManager() {
	glfwSetKeyCallback(keyCallback);
	glfwSetCharCallback(charCallback);
	glfwSetMousePosCallback(mPosCallback);
	glfwSetMouseButtonCallback(mButtonCallback);
	glfwSetScrollCallback(mScrollCallback);
	glfwSetWindowSizeCallback(resizeCallback);

	DetectControllers();
}

InputManager::~InputManager() {
	for (int i = 0; i<status.controllers.size(); i++) {
		delete status.controllers[i];
	}
	status.controllers.clear();
}

Controller::Controller(int _id) {
	this->id = _id;
	this->num_axes = glfwGetJoystickParam(this->id, GLFW_AXES);
	this->num_buttons = glfwGetJoystickParam(this->id, GLFW_BUTTONS);

	// I'm not entirely sure this is required, but it's probably not a bad idea.
	this->axes = new float[this->num_axes];
	this->buttons = new KeyState[this->num_buttons];
	this->buttons_raw = new unsigned char[this->num_buttons];
	this->timestamp = new double[this->num_buttons];

	// initial update.
	glfwGetJoystickButtons(this->id, this->buttons_raw, this->num_buttons);
	glfwGetJoystickPos(this->id, this->axes, this->num_axes);

	Log->Print("Creating controller");
}

Controller::~Controller() {
	Log->Print("Destroying controller %d", this->id+1);

	delete[] this->axes;
	delete[] this->buttons;
	delete[] this->buttons_raw;
}

void InputManager::DetectControllers() {
	// find all our joysticks
	for (int i = 0; i<GLFW_JOYSTICK_LAST; i++) {
		int present = glfwGetJoystickParam(i, GLFW_PRESENT);
		if (present) {
			// joystick/controller was found - register it and get info.
			Controller* current = new Controller(i);
			status.controllers.push_back(current);
		}
	}

	// log what we found
	if (status.controllers.size() > 0) {
		Log->Print("Controllers found:");
		for (int i = 0; i<status.controllers.size(); i++)
			Log->InlinePrint(
				"\tController %d (id=%d, buttons=%d, axes=%d)\n", i+1,
				status.controllers[i]->id,
				status.controllers[i]->num_buttons,
				status.controllers[i]->num_axes
			);
	}
}

void InputManager::UpdateControllers() {
	// update all joystick buttons + axes.
	for (int i = 0; i<status.controllers.size(); i++) {
		// store old values for comparison
		unsigned char old_buttons[status.controllers[i]->num_buttons];
		memcpy(old_buttons, status.controllers[i]->buttons_raw, sizeof(old_buttons));

		// update the current values
		glfwGetJoystickButtons(
			status.controllers[i]->id,
			status.controllers[i]->buttons_raw,
			status.controllers[i]->num_buttons
		);
		glfwGetJoystickPos(
			status.controllers[i]->id,
			status.controllers[i]->axes,
			status.controllers[i]->num_axes
		);

		// and now set timestamp if a button has changed.
		for (int j = 0; j<status.controllers[i]->num_buttons; j++) {
			if (old_buttons[j] != status.controllers[i]->buttons_raw[j]) {
				double cur_time = glfwGetTime();
				// this should send an IEvent, too.
				status.controllers[i]->timestamp[j] = cur_time;
			}
		}
	}
}

void InputManager::Update() {
	glfwPollEvents();

	// controllers aren't driven by a callback - so we have to do it here.
	UpdateControllers();
}

void Input_Binding() {
	Lua->PushNumber("Key_Escape", 27);
	Lua->PushNumber("Key_Return", 294);
}
