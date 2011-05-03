#include <cstdio>
#include <cstdlib>
#include <cstdarg>

#include <GL/glfw3.h>
#include "../GameManager.h"
#include "Logger.h"

Logger *Log = NULL;

// TODO: Log to file(s)
Logger::Logger() : m_cColor(FG_RED), m_bUseColors(false)
{
}

std::string Logger::SPrint(const char *in, ...)
{
	va_list va;
	char staticbuf[1024];
	char *buf = staticbuf;
	va_start(va, in);
	unsigned int need = vsnprintf(buf, sizeof(staticbuf), in, va) + 1;
	
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

void Logger::UseColors(bool enabled)
{
	m_bUseColors = enabled;
}

void Logger::SetColor(ConsoleColor color)
{
	m_cColor = color;
}

void Logger::Print(const char *in, ...)
{
	va_list va;
	char staticbuf[1024];
	char *buf = staticbuf;
	va_start(va, in);
	unsigned int need = vsnprintf(buf, sizeof(staticbuf), in, va) + 1;
	
	if (need > sizeof(staticbuf))
	{
		// staticbuf wasn't large enough, malloc large enough
		buf = (char *) malloc(need);
		va_start(va,in);
		vsnprintf(buf, need, in, va);
	}
	
	va_end(va);
	
	if (m_bUseColors)
		printf("[%0.3f] \x1b[%d;1m%s\x1b[0m\n", glfwGetTime(), m_cColor, buf);
	else
		printf("[%0.3f] %s\n", glfwGetTime(), buf);
		
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
	unsigned int need = vsnprintf(buf, sizeof(staticbuf), in, va) + 1;
	
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

/**
 * @file
 * @author Colby Klein, Jarno Purontakanen (c) 2011
 * @section LICENSE
 * 
 * All rights reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, and/or sell copies of the Software, and to permit persons to
 * whom the Software is furnished to do so, provided that the above
 * copyright notice(s) and this permission notice appear in all copies of
 * the Software and that both the above copyright notice(s) and this
 * permission notice appear in supporting documentation.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF
 * THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS
 * INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
