#ifndef _Log_H_
#define _Log_H_

#include <string>
#include <cstdio>

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
		
	std::string Format(const char *in, ...);
	
	// Only shows when DEBUG is defined (either via build or prefs)
	void Debug(const char *in, ...);
	void Debug(std::string input);
	
	// Log to file, print in terminal
	void Info(const char *in, ...);
	void Info(std::string in);
	
	// Same as Info, but with WARNING
	void Warn(const char* in, ...);
	void Warn(std::string in);
	
	void UseColors(bool enabled);

private:
	std::string m_file;
	FILE* file;

	ConsoleColor m_cColor;
	bool m_bUseColors;
};

extern Logger *LOG;

#endif
