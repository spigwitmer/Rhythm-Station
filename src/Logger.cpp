#include <cstdio>
#include <cstdlib>
#include <cstdarg>

#include <GL/glfw3.h>
#include "GameManager.h"
#include "Logger.h"

Logger* Log = NULL;

Logger::Logger()
{
#if 0
	// doesn't work
	if (getenv("BASH") != NULL)
		bEnableColors = true;
	else
		bEnableColors = false;
#endif
}

// don't forget to define _DEBUG_!
void Logger::DebugPrint(std::string input)
{
	if (Game->IsDebugMode())
		Print("%s (debug)", input.c_str());
}

std::string Logger::SPrint(const char* in, ...)
{
	va_list va;
	char staticbuf[1024];
	char *buf = staticbuf;

	va_start(va, in);
	unsigned int need = vsnprintf(buf, sizeof(staticbuf),in, va) + 1;
	if (need > sizeof(staticbuf))
	{
		// staticbuf wasn't large enough, malloc large enough
		buf = (char *) malloc(need);
		va_start(va,in);
		vsnprintf(buf, need, in, va);
	}
	va_end(va);

	std::string ret(buf);

	// free if we had to malloc more space
	if (buf != staticbuf)
		free(buf);

	return ret;
}

void Logger::Print(std::string in)
{
	Print(in.c_str());
}

void Logger::Print(const char *in, ...)
{
	va_list va;
	char staticbuf[1024];
	char *buf = staticbuf;

	va_start(va, in);
	unsigned int need = vsnprintf(buf, sizeof(staticbuf),in, va) + 1;
	if (need > sizeof(staticbuf))
	{
		// staticbuf wasn't large enough, malloc large enough
		buf = (char *) malloc(need);
		va_start(va,in);
		vsnprintf(buf, need, in, va);
	}
	va_end(va);

#if 0
	if (bEnableColors)
		printf("\x1b[%d;1m[%0.3f]\x1b[0m %s\n", FG_RED, glfwGetTime(),buf);
	else
#endif
		printf("[%0.3f] %s\n", glfwGetTime(),buf);

	// free if we had to malloc more space
	if (buf != staticbuf)
		free(buf);
}

void Logger::InlinePrint(const char *in, ...)
{
	va_list va;
	char staticbuf[1024];
	char *buf = staticbuf;

	va_start(va, in);
	unsigned int need = vsnprintf(buf, sizeof(staticbuf),in, va) + 1;
	if (need > sizeof(staticbuf))
	{
		// staticbuf wasn't large enough, malloc large enough
		buf = (char *) malloc(need);
		va_start(va,in);
		vsnprintf(buf, need, in, va);
	}
	va_end(va);

	printf("%s", buf);

	// free if we had to malloc more space
	if (buf != staticbuf)
		free(buf);
}
