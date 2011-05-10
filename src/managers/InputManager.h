#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "Inputs.h"
#include "ButtonState.h"
#include "utils/Singleton.h"
#include <glm/glm.hpp>

class InputManager : public Singleton<InputManager>
{
public:
	InputManager();
	virtual ~InputManager();

	void DiscardQueue();

	void Update();

	// Grabs an input from the queue
	ButtonState *GetNextInput();

	// Single button testing
	ButtonState *GetButton(RSButtons b) const;

	// Simplistic mouse grabbing
	glm::ivec2 &GetMousePos() const;
};

#endif // INPUT_MANAGER_H

/**
 * @file
 * @author Matt Vandermeulen, Colby Klein (c) 2011
 * @section LICENSE
 * 
 * This program is licensed under the terms of the MIT license.
 * The full text can be found in License.txt.
 */
