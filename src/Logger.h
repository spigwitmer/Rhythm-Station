#ifndef _Log_H_
#define _Log_H_

#include <string>
#include <iostream>
#include <fstream>

class Logger {
public:
	Logger();
	virtual ~Logger();

	void Write();
	void DebugPrint(std::string input);
	void Print(std::string input);

	std::string path;

private:
	std::fstream logFile;
};

extern Logger* Log;

#endif
