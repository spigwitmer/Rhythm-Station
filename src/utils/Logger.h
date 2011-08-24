#ifndef _Log_H_
#define _Log_H_

#include <string>

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

	// Kills program if debug.
	void Fatal(const char* in, ...);
	void Fatal(std::string in);
};

extern Logger *LOG;

#endif
