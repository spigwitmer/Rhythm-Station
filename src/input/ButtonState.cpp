#include "ButtonState.h"

ButtonState::ButtonState(unsigned iButton) :
	m_iButton(iButton),
	m_bDown(false)
{}

void ButtonState::Press(bool b)
{
	m_bDown = b;
	m_Timer.Touch();
}

double ButtonState::GetHeldTime() const
{
	if(!m_bDown)
		return 0;

	// This is simple, <3 timer
	return m_Timer.Ago();
}
