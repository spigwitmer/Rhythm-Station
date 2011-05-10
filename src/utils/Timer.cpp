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

std::string Timer::StrAgo() const
{
	std::ostringstream str;
	str << this->Ago();

	return str.str();
}

/**
 * @file
 * @author Colby Klein (c) 2011
 * @section LICENSE
 * 
 * This program is licensed under the terms of the MIT license.
 * The full text can be found in License.txt.
 */
