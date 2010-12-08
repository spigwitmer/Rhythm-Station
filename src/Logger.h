#ifndef _Log_H_
#define _Log_H_

#include <string>

class Logger {
public:
	void DebugPrint(std::string input);
	void Print(std::string in, ...);
	void InlinePrint(std::string in, ...);
};

extern Logger* Log;

#endif
