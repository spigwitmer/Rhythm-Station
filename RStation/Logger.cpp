#include <fstream>
#include <sstream>
#include <GL/glfw.h>
#include <stdint.h>
#include "Logger.h"
#include "FileManager.h"

Logger* Log = NULL;

Logger::Logger()
{
	if (!File->FileExists(path))
		path = File->GetFile("RStation-log.txt");
	logFile.open(path.c_str(), std::fstream::out);
	Print("Starting RStation");
	Print("-------------------------------------------------------");
}

Logger::~Logger()
{
	logFile.close();
}

int logcount;

void Logger::Write()
{
	Print("[Log::Write] Flushing log to disk.");
	logcount = 0;
	logFile.flush();
}

// don't forget to define _DEBUG_!
void Logger::DebugPrint(std::string input)
{
#if _DEBUG_
	Print(input+std::string(" (debug)"));
#endif
}

void Logger::Print(std::string input)
{
	// don't fill up our buffer too much between writes.
	if (++logcount > 500)
		Write();

	std::ostringstream out;
	out << "[";
	char buf[16] = "";
	sprintf(buf, "%0.3f", glfwGetTime());
	out << buf;
	out << "] ";
	out << input;
	out << "\n";
	
	std::cout << out.str();
	logFile << out.str();
}	
