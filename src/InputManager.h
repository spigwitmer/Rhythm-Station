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

// this should probably only handle mapped game buttons - not raw input.
struct IEvent2 {
	Controller* controllers;
	int* key;
	int num_keys;

	double* timestamp;
};

class InputManager {
public:
	InputManager();
	virtual ~InputManager();

	void Update();

	std::vector<Controller> controllers;

private:
	void DetectControllers();
	void UpdateControllers();
//	InputState m_inputState;
};

extern InputManager* Input;

#endif
