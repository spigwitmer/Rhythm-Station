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
	// free the memory allocated by the controller inputs
	for (int i = 0; i<controllers.size(); i++) {
		delete[] controllers[i].axes;
		delete[] controllers[i].buttons;
	}
	controllers.clear();
}

void InputManager::DetectControllers() {
	// find all our joysticks
	for (int i = 0; i<GLFW_JOYSTICK_LAST; i++) {
		int present = glfwGetJoystickParam(i, GLFW_PRESENT);
		if (present) {
			// joystick/controller was found - register it and get info.
			Controller current;
			current.id = i;

			current.num_axes = glfwGetJoystickParam(i, GLFW_AXES);
			current.num_buttons = glfwGetJoystickParam(i, GLFW_BUTTONS);

			// I'm not entirely sure this is required, but it's probably not a bad idea.
			current.axes = new float[current.num_axes];
			current.buttons = new unsigned char[current.num_buttons];
			current.timestamp = new double[current.num_buttons];

			// initial update.
			glfwGetJoystickButtons(i, current.buttons, current.num_buttons);
			glfwGetJoystickPos(i, current.axes, current.num_axes);

			controllers.push_back(current);
		}
	}

	// log what we found
	if (controllers.size() > 0) {
		Log->Print("Controllers found:");
		for (int i = 0; i<controllers.size(); i++)
			Log->InlinePrint("\tController %d (id=%d, buttons=%d, axes=%d)\n", i+1,
				controllers[i].id, controllers[i].num_buttons, controllers[i].num_axes);
	}
}

void InputManager::UpdateControllers() {
	// update all joystick buttons + axes.
	for (int i = 0; i<controllers.size(); i++) {
		// store old values for comparison
		unsigned char old_buttons[controllers[i].num_buttons];
		memcpy(old_buttons, controllers[i].buttons, sizeof(old_buttons));

		// update the current values
		glfwGetJoystickButtons(controllers[i].id, controllers[i].buttons, controllers[i].num_buttons);
		glfwGetJoystickPos(controllers[i].id, controllers[i].axes, controllers[i].num_axes);

		// and now set timestamp if a button has changed.
		for (int j = 0; j<controllers[i].num_buttons; j++) {
			if (old_buttons[j] != controllers[i].buttons[j]) {
				double cur_time = glfwGetTime();
				// printf("button: %d (%d -> %d @ %f)\n", j, old_buttons[j], controllers[i].buttons[j], cur_time);
				controllers[i].timestamp[j] = cur_time;
			}
		}
	}
}

void InputManager::Update() {
	glfwPollEvents();

	UpdateControllers();
}

void Input_Binding() {
	Lua->PushNumber("Key_Escape", 27);
	Lua->PushNumber("Key_Return", 294);
}
