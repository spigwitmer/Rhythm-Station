#include <cstdio>
#include <cstdlib>
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
	char staticbuf[1024];
	char *buf = staticbuf;

	va_start(va, in);
	unsigned int need = vsnprintf(buf, sizeof(staticbuf),in.c_str(), va) + 1 ;
	if (need > sizeof(staticbuf)) {
		// staticbuf wasn't large enough, malloc large enough
		buf = (char *) malloc(need);
		va_start(va,in);
		vsnprintf(buf, need, in.c_str(), va);
	}
	va_end(va);

	printf("\x1b[%d;1m[%0.3f]\x1b[0m %s\n", FG_RED, glfwGetTime(),buf);

	// free if we had to malloc more space
	if (buf != staticbuf) {
		free(buf);
	}
}

void Logger::InlinePrint(std::string in, ...) {
	va_list va;
	char staticbuf[1024];
	char *buf = staticbuf;

	va_start(va, in);
	unsigned int need = vsnprintf(buf, sizeof(staticbuf),in.c_str(), va) + 1 ;
	if (need > sizeof(staticbuf)) {
		// staticbuf wasn't large enough, malloc large enough
		buf = (char *) malloc(need);
		va_start(va,in);
		vsnprintf(buf, need, in.c_str(), va);
	}
	va_end(va);

	printf("%s", buf);

	// free if we had to malloc more space
	if (buf != staticbuf) {
		free(buf);
	}
}
