#ifndef BUTTON_STATE_H
#define BUTTON_STATE_H

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

#endif // BUTTON_STATE_H
