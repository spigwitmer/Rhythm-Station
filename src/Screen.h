#ifndef _SCREEN_H_
#define _SCREEN_H_

#include <vector>
#include "ResourceManager.h"
#include "InputManager.h"
#include "Object.h"

class Screen {
public:
	Screen();
	virtual ~Screen();

	void AddObject(Object* object);
	void Register() { }

//	virtual void Input(const IEvent &e);
	virtual void Update(float deltaTime);
	virtual void Draw();

private:
	std::vector<Object*> m_vpObjects;
};

#endif
