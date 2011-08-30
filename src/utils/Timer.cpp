#include "Timer.h"
#include <GL/glfw3.h>
#include <sstream>

Timer::Timer() : m_iStart(glfwGetTime()) {}

void Timer::Touch()
{
	// Reset
	m_iStart = glfwGetTime();
}

double Timer::Ago() const
{
	return glfwGetTime() - m_iStart;
}
