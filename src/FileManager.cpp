#include "FileManager.h"
#include "Logger.h"

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#endif

#if defined(__WINDOWS__) || defined(_WIN32)
#include <direct.h>
#endif

// prevent visual studio warning
#ifdef _MSC_VER
#define getcwd _getcwd
#endif

#include <stdio.h>
#include <fstream>
#include <sys/stat.h>
#include "PreferencesFile.h"

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
// For OS X, get stuff out of the application bundle (where it is expected to reside)
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
	_path += "/Contents/Resources";
	chdir(_path.c_str());
#endif
}

// this will probably be removed later.
std::string FileManager::GetWorkingDirectory()
{
	SetWorkingDirectory();
	char path[1024] = "";
	
	if (getcwd(path, 1024) != NULL)
		return std::string(path) + "/";
	else
		return std::string("./");
}

std::string FileManager::GetFile(std::string _path)
{
	if (_path[0] != '/' && Preferences != NULL)
		_path = Log->SPrint("Themes/%s/%s",
			Preferences->GetValue("Game", "CurrentTheme"),
			_path.c_str());
	else
		_path = _path.substr(1);

	
	return GetWorkingDirectory() + _path;
}

std::string FileManager::GetFileContents(std::string _path)
{
	std::string out, buf;
	std::ifstream file(_path.c_str());
	
	if (!file.is_open())
	{
		Log->Print("Error opening %s for writing", _path.c_str());
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
