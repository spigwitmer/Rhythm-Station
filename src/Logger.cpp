#include <sstream>
#include <iostream>
#include <cstdio>
#include <cstdarg>

#include <GL/glfw3.h>
#include "GameManager.h"
#include "Logger.h"

Logger* Log = NULL;

// don't forget to define _DEBUG_!
void Logger::DebugPrint(std::string input) {
	if (Game->IsDebugMode())
		Print(input+std::string(" (debug)"));
}

void Logger::Print(std::string in, ...) {
	va_list va;

	// there has to be a better way to handle this. a long enough print will cause a crash.
	char buf[in.length() + 2048];
	va_start(va, in.c_str()); // throws an warning! ignore it.
	vsprintf(buf, in.c_str(), va);
	va_end(va);

	// timestamp
	char buf2[16] = "";
	sprintf(buf2, "%0.3f", glfwGetTime());

	std::ostringstream out;
	out << "[" << buf2 << "] " << buf << "\n";

	// tried passing the va_list and making InlinePrint handle this, didn't behave right.
	std::cout << out.str();
}	

void Logger::InlinePrint(std::string in, ...) {
	va_list va;

	va_start(va, in.c_str());
	char buf[in.length() + 2048];
	vsprintf(buf, in.c_str(), va);
	va_end(va);

	std::cout << buf;
}	
