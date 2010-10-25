#ifndef _INPUT_MANAGER_H_
#define _INPUT_MANAGER_H_

/*
 * should hold the current input state at all times and update for differences
 * to prevent weird things from missing fields.
*/
class InputManager {
public:
	InputManager();
	virtual ~InputManager();

	void Update();
private:
//	InputState m_inputState;
};

extern InputManager* Input;

#endif
