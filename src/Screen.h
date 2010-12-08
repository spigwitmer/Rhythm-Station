#ifndef _SCREEN_H_
#define _SCREEN_H_

#include <vector>
#include "ResourceManager.h"
#include "InputManager.h"
#include "Object.h"
#include "Sound.h"

class Screen {
public:
	Screen();
	virtual ~Screen();

	void AddObject(Object* object);
	void AddObject(Sound* sound);
	void Register() { }

	virtual void Input(const IEvent &e);
	void Update(float deltaTime);
	void Draw();

private:
	std::vector<Object*> m_vpObjects;
	std::vector<Sound*> m_vpSounds;
};

#endif
