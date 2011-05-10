#include <cstdio>
#include <cstdlib>
#include <cstdarg>

#include <GL/glfw3.h>
#include "Logger.h"

using namespace std;

Logger *LOG = NULL;

/*
 * We'll need to do this in several functions and making it a normal function
 * doesn't behave properly. Macros to the rescue!
 */
#define FORMAT(str) \
{ \
	va_list va; \
	char staticbuf[1024]; \
	char *buf = staticbuf; \
	va_start(va, in); \
	unsigned int need = vsnprintf(buf, sizeof(staticbuf), in, va) + 1; \
	\
	if (need > sizeof(staticbuf)) \
	{ \
		/* staticbuf wasn't large enough, malloc large enough */ \
		buf = (char *) malloc(need); \
		va_start(va,in); \
		vsnprintf(buf, need, in, va); \
	} \
	\
	va_end(va); \
	str = string(buf); \
	\
	/* free if we had to malloc more space */ \
	if (buf != staticbuf) \
		free(buf); \
}

// TODO: Log to file(s)
Logger::Logger() : m_cColor(FG_RED), m_bUseColors(false) { }

void Logger::UseColors(bool enabled)
{
	m_bUseColors = enabled;
}

string Logger::Format(const char *in, ...)
{
	string ret;
	FORMAT(ret);
	
	return ret;
}

void Logger::Info(const char *in, ...)
{
	string ret;
	FORMAT(ret);
	
	if (m_bUseColors)
		printf("[%0.3f] \x1b[%d;1m%s\x1b[0m\n", glfwGetTime(), m_cColor, ret.c_str());
	else
		printf("[%0.3f] %s\n", glfwGetTime(), ret.c_str());
}

void Logger::Info(string in)
{
	Info(in.c_str());
}

/**
 * @file
 * @author Colby Klein, Jarno Purontakanen (c) 2011
 * @section LICENSE
 * 
 * This program is licensed under the terms of the MIT license.
 * The full text can be found in License.txt.
 */
