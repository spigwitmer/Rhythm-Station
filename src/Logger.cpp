#include <sstream>
#include <iostream>
#include <cstdio>

#include <GL/glfw3.h>
#include "GameManager.h"
#include "Logger.h"

Logger* Log = NULL;

// don't forget to define _DEBUG_!
void Logger::DebugPrint(std::string input)
{
	if (Game->IsDebugMode())
		Print(input+std::string(" (debug)"));
}

void Logger::Print(std::string input)
{
	char buf[16] = "";
	sprintf(buf, "%0.3f", glfwGetTime());

	std::ostringstream out;
	out << "[" << buf << "] " << input << "\n";

	std::cout << out.str();
}	
