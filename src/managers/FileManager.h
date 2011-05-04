#ifndef _File_Manager_H_
#define _File_Manager_H_

#include <string>
#include <vector>

//class FileManager : public Singleton<FileManager>
namespace FileManager
{
	bool FileExists(std::string, std::string ext = "");
	void SetWorkingDirectory();
	
	std::string GetWorkingDirectory();
	std::string GetFile(std::string);
	std::string GetThemeFile(std::string);
	std::string GetFileContents(std::string);
	std::vector<std::string> GetDirectoryListing(std::string dir, std::string ext = "");
};

#endif
