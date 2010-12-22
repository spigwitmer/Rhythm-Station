#include "Timer.h"
#include <GL/glfw3.h>
#include <sstream>

Timer::Timer() : start_time(glfwGetTime()) { }

void Timer::Touch()
{
	start_time = glfwGetTime();
}

double Timer::Ago()
{
	return glfwGetTime() - start_time;
}

// convenient sometimes.
std::string Timer::strAgo()
{
	std::ostringstream str;
	str << this->Ago();
	return str.str();
}
