#include <cstdio>
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

	// note: asprintf and vasprintf might not be friendly to non-gcc compilers.
	char* buf, *buf2;
	asprintf(&buf, "%0.3f", glfwGetTime());

	// this throws a warning, ignore it.
	va_start(va, in.c_str());
	vasprintf(&buf2, in.c_str(), va);
	va_end(va);

	printf("[%s] %s\n", buf, buf2);

	free(buf);
	free(buf2);
}

void Logger::InlinePrint(std::string in, ...) {
	va_list va;

	char* buf;
	va_start(va, in.c_str());
	vasprintf(&buf, in.c_str(), va);
	va_end(va);

	printf("%s", buf);

	free(buf);
}
