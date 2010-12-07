#ifndef _INPUT_MANAGER_H_
#define _INPUT_MANAGER_H_

#include <vector>

/*
 * should hold the current input state at all times and update for differences
 * to prevent weird things from missing fields.
*/
struct Controller {
	int id;
	float* axes;
	int num_axes;

	unsigned char* buttons;
	int num_buttons;

	// timestamps for button presses.
	double* timestamp;
};

class InputManager {
public:
	InputManager();
	virtual ~InputManager();

	void Update();

	std::vector<Controller> controllers;

private:
//	InputState m_inputState;
};

extern InputManager* Input;

#endif
