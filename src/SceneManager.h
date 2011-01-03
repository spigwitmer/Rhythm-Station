#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

#include "InputManager.h"
#include "Screen.h"

namespace SceneManager
{
	void PushScreen();
	void PopScreen();

	Screen* GetTopScreen();
	void SendInput(IEvent &e);
	void Update(float deltaTime);
	void Draw();

	void Clear();
};

#endif
