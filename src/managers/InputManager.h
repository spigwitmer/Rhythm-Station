#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "Inputs.h"
#include "ButtonState.h"
#include "utils/Singleton.h"

// These vectors are getting a tryout!
#include <glm/glm.hpp>

class InputManager : public Singleton<InputManager>
{
public:
	// Construction, destruction
	InputManager();
	virtual ~InputManager();

	// Helper function that discards the queue
	void DiscardQueue();

	// Called every, err.. Update..
	void Update();

	// Grabs an input from the queue
	ButtonState *GetNextInput();

	// Single button testing
	ButtonState *GetButton(RSButtons b) const;

	// Simplistic mouse grabbing
	glm::ivec2 &GetMousePos() const;
};

#endif // INPUT_MANAGER_H
