#include <GL/glfw3.h>
#include <stdio.h>
#include "InputManager.h"
#include "GameManager.h"
#include "Logger.h"

InputManager* Input = NULL;

std::string str;

// keyboard. key for specials, char for text input and such.
void keyCallback(GLFWwindow window, int key, int state) {
	if (state == GLFW_PRESS) {
//		double timestamp = glfwGetTime();
//		printf("[%01.3f] %d\n", timestamp, key);

		if (key == 294) {
			Log->Print(str);
			str.clear();
		}
	}
}

void charCallback(GLFWwindow window, int key) {
	char buf[2];
	sprintf(buf, "%c", key);
	str.push_back(buf[0]);
}

// mouse actions
void mPosCallback() {

}

void mButtonCallback() {

}

void mScrollCallback() {

}

// on window resize
void resizeCallback() {

}

// TODO
InputManager::InputManager() {
	glfwSetKeyCallback(Game->GetWindow(), keyCallback);
	glfwSetCharCallback(Game->GetWindow(), charCallback);
//	glfwSetMousePosCallback(Game->GetWindow(), mPosCallback);
//	glfwSetMouseButtonCallback(Game->GetWindow(), mButtonCallback);
//	glfwSetScrollCallback(Game->GetWindow(), mScrollCallback);
//	glfwSetWindowSizeCallback(Game->GetWindow(), resizeCallback);
}

InputManager::~InputManager() {
	
}

void InputManager::Update() {
	glfwPollEvents();
}
