#ifndef _Log_H_
#define _Log_H_

#include <string>

class Logger {
public:
	void DebugPrint(std::string input);
	void Print(std::string input);
};

extern Logger* Log;

#endif
