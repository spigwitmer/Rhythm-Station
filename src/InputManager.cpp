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

// TODO
InputManager::InputManager() {
	glfwSetKeyCallback(keyCallback);
	glfwSetCharCallback(charCallback);
	glfwSetMousePosCallback(mPosCallback);
	glfwSetMouseButtonCallback(mButtonCallback);
	glfwSetScrollCallback(mScrollCallback);
	glfwSetWindowSizeCallback(resizeCallback);
}

InputManager::~InputManager() {

}

void InputManager::Update() {
	glfwPollEvents();
}

void Input_Binding() {
	Lua->PushNumber("Key_Escape", 27);
	Lua->PushNumber("Key_Return", 294);
}
