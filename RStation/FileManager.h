#ifndef _File_Manager_H_
#define _File_Manager_H_

#include <string>

class FileManager
{
public:
	bool FileExists(std::string);
	void SetWorkingDirectory();
	std::string GetWorkingDirectory();
	std::string GetFile(std::string);
	std::string GetThemeFile(std::string);
	std::string GetFileContents(std::string);
};

extern FileManager* File;

#endif
