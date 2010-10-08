#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

#include "RStation.h"
#include "InputManager.h"
#include "Screen.h"

class SceneManager
{
public:
	void PushScreen();
	void PopScreen();

	Screen* GetTopScreen();
	void SendInput(IEvent &e);
	void Update(float deltaTime);
	void Draw();

	void Clear();

private:
	std::vector<Screen*> vpScreens;
};

extern SceneManager* Scene;

#endif
