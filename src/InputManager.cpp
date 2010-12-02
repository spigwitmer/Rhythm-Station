#include <GL/glfw3.h>
#include <stdio.h>
#include "InputManager.h"
#include "GameManager.h"
#include "LuaManager.h"
#include "Logger.h"
#include "globals.h"

InputManager* Input = NULL;

std::string str;

struct Controller {
	int id;
	float* axes;
	int num_axes;

	unsigned char* buttons;
	int num_buttons;
};

std::vector<Controller> controllers;

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
		printf("key: %d\n", key);
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
			printf("\tController %d (id=%d, buttons=%d, axes=%d)\n", i+1,
				controllers[i].id, controllers[i].num_buttons, controllers[i].num_axes);
		printf("\n");
	}
}

InputManager::~InputManager() {
	// free the memory allocated by the controller inputs
	for (int i = 0; i<controllers.size(); i++) {
		delete[] controllers[i].axes;
		delete[] controllers[i].buttons;
	}
	controllers.clear();
}

void InputManager::Update() {
	glfwPollEvents();

	// update all joystick buttons + axes
	for (int i = 0; i<controllers.size(); i++) {
		glfwGetJoystickButtons(controllers[i].id, controllers[i].buttons, controllers[i].num_buttons);
		glfwGetJoystickPos(controllers[i].id, controllers[i].axes, controllers[i].num_axes);
	}
}

void Input_Binding() {
	Lua->PushNumber("Key_Escape", 27);
	Lua->PushNumber("Key_Return", 294);
}
