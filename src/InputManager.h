#ifndef _INPUT_MANAGER_H_
#define _INPUT_MANAGER_H_

#include <vector>
#include <string>
#include "Type.h"

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

struct Mouse {
	KeyState* buttons;
	int* buttons_raw;

	// coords
	int x, y;

	// normalized coords
	float nx, ny;
};

// this should probably only handle mapped game buttons - not raw input.
struct IEvent {
	std::vector<Controller*> controllers;
	std::string cur_string;

	vec2 mouse_pos;
	vec2 scroll;

	KeyState* keys;
	int num_keys;

	double* timestamp;
};

class InputManager {
public:
	InputManager();
	virtual ~InputManager();

	void Update();
	void SendEvent();

	/*
	 * Keep the current state here and send it every time there's an update.
	 * This needs to be public so that the callbacks can update it.
	 */
	IEvent status;

private:
	void DetectControllers();
	void UpdateControllers();

	bool queuedUpdate;
};

extern InputManager* Input;

#endif
