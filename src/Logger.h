#ifndef _Log_H_
#define _Log_H_

#include <string>

enum ConsoleColor
{
	FG_BLACK	= 30,
	FG_RED		= 31,
	FG_GREEN	= 32,
	FG_BROWN	= 33,
	FG_BLUE		= 34,
	FG_MAGENTA	= 35,
	FG_CYAN		= 36,
	FG_WHITE	= 37,
	FG_YELLOW	= 38
};

class Logger
{
public:
	Logger();

	void DebugPrint(std::string input);
	void Print(const char *in, ...);
	void Print(std::string in);
	void InlinePrint(const char *in, ...);

private:
#if 0
	bool bEnableColors;
#endif
};

extern Logger* Log;

#endif
