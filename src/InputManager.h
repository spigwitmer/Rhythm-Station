#ifndef _INPUT_MANAGER_H_
#define _INPUT_MANAGER_H_

#include <vector>

enum KeyState {
	KEY_NONE = 0,
	KEY_PRESSED,
	KEY_HELD,
	KEY_LETGO
};

struct Controller {
	Controller(int id);
	virtual ~Controller();

	int id;
	float* axes;
	int num_axes;

	KeyState* buttons;
	unsigned char* buttons_raw;
	int num_buttons;

	// timestamps for button presses.
	double* timestamp;
};

// this should probably only handle mapped game buttons - not raw input.
struct IEvent {
	std::vector<Controller*> controllers;

	KeyState* keys;
	int num_keys;

	double* timestamp;
};

class InputManager {
public:
	InputManager();
	virtual ~InputManager();

	void Update();

private:
	void DetectControllers();
	void UpdateControllers();

	// keep the current state here and send it every time there's an update.
	IEvent status;
};

extern InputManager* Input;

#endif
