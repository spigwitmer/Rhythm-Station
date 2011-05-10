#ifndef _INPUT_MANAGER_H_
#define _INPUT_MANAGER_H_

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "utils/Singleton.h"

enum KeyState
{
	KEY_NONE = 0,
	KEY_PRESSED,
	KEY_HELD,
	KEY_LETGO
};

struct Controller
{
	Controller(int id);
	virtual ~Controller();
	
	int id;
	float *axes;
	int num_axes;
	
	KeyState *buttons;
	unsigned char *buttons_raw;
	int num_buttons;
	
	// timestamps for button presses.
	double *timestamp;
};

struct Mouse
{
	KeyState *buttons;
	
	// coords
	int x, y;
	
	// normalized coords
	float nx, ny;
	
	glm::vec2 mouse_pos;
	glm::vec2 scroll;
};

// this should probably only handle mapped game buttons - not raw input.
struct IEvent
{
	std::vector<Controller *> controllers;
	
	Mouse mouse;
	
	KeyState *keys;
	int num_keys;
	
	double *timestamp;
	
	int last_type;
};

class InputManager : public Singleton<InputManager>
{
public:
	InputManager();
	virtual ~InputManager();
	
	void Connect();
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

#endif
