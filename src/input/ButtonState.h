#pragma once

#include "utils/Timer.h"

class ButtonState
{
public:
	ButtonState(unsigned iButton);

	unsigned GetButton() const	{ return m_iButton; }

	bool IsDown() const { return m_bDown; }
	void Press(bool b);

	// Time since Press(true)
	double GetHeldTime() const;

private:
	const unsigned m_iButton;
	bool m_bDown;

	Timer m_Timer;
};

/**
 * Matt Vandermeulen, Colby Klein (c) 2011
 * Licensed under the terms of the MIT license. See License.txt.
 */
