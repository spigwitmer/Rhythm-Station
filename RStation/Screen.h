#ifndef _SCREEN_H_
#define _SCREEN_H_

#include "RStation.h"
#include "InputManager.h"
#include "Actor.h"

class Screen
{
public:
	Screen();
	virtual ~Screen();

	void AddActor(Actor* actor);
	
	virtual void Input(const IEvent &e);
	virtual void Update(float deltaTime);
	virtual void Draw();
private:
	std::vector<Actor*> vpActors;

//	GLuint alphaLoc;
};

#endif