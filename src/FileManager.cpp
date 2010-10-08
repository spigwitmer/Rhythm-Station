#include "FileManager.h"
#include "Logger.h"

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#endif

#ifdef __WINDOWS__
#include <direct.h>
#endif

// prevent visual studio warning
#ifdef _MSC_VER
	#define getcwd _getcwd
#endif

#include <stdio.h>
#include <fstream>
#include <sys/stat.h>

FileManager* File = NULL;

/*
 * This may not benefit from being a class like Logger does, but I'll give it a shot.
 */
bool FileManager::FileExists(std::string _file)
{
	struct stat stFileInfo;
	int iStat;

	iStat = stat(_file.c_str(),&stFileInfo);
	if (iStat == 0)
		return true;
	return false;
}
void FileManager::SetWorkingDirectory()
{
	// gross
#ifdef __APPLE__
	/*
	 * This function will locate the path to our application on OS X,
	 * unlike windows you cannot rely on the current working directory
	 * for locating your configuration files and resources.
	 */
	char path[1024];
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	assert(mainBundle); // make sure nothing is terribly wrong here.
	CFURLRef mainBundleURL = CFBundleCopyBundleURL(mainBundle);
	CFStringRef cfStringRef = CFURLCopyFileSystemPath(mainBundleURL, kCFURLPOSIXPathStyle);
	CFStringGetCString(cfStringRef, path, 1024, kCFStringEncodingUTF8);
	CFRelease(mainBundleURL);
	CFRelease(cfStringRef);
	std::string _path = path;
	size_t pos = _path.find_last_of('/');
	if (pos != std::string::npos)
		_path = _path.substr(0,pos);
	chdir(_path.c_str());
#endif
}
std::string FileManager::GetWorkingDirectory()
{
	SetWorkingDirectory();
	char path[1024] = "./";
	getcwd(path, 1024);
	return std::string(path) + "/";
}
std::string FileManager::GetFile(std::string _path)
{
	return GetWorkingDirectory() + _path;
}
std::string FileManager::GetFileContents(std::string _path)
{
	std::string out, buf;
	std::ifstream file(_path.c_str());
	
	if (!file.is_open())
	{
		Log->Print("Error opening " + _path + " for writing");
		return std::string();
	}
	
	while (!file.eof())
	{
		getline(file, buf);
		out += buf;
		out += "\n";
	}
	
	return out;
}
