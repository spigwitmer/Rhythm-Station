#include "FileManager.h"
#include <glsw.h>
#include <string>

using namespace std;

FileManager::FileManager() {
	glswInit();
	glswSetPath("Data/Shaders/", ".glsl");
	
	/*
	 * Enforce 120 for GL 2.x, and 150 for 3.x.
	 * We only actually support GL 2.1 and 3.2, 
	 */
	glswAddDirectiveToken("GL21", "#version 120");		
	glswAddDirectiveToken("GL32", "#version 150");
}

FileManager::~FileManager() {
	glswShutdown();
}

void FileManager::Mount(string internal, string real)
{
	// TODO
}

vector<string> GetDirectoryListing(string path)
{
	vector<string> ret;
	
	return ret;
}











#include "FileManager.h"
#include "utils/Logger.h"

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#endif

#if defined(__WINDOWS__) || defined(_WIN32)
#include <windows.h>
#include <direct.h>
#endif

// prevent visual studio warning
#ifdef _MSC_VER
#define getcwd _getcwd
#endif

#include <errno.h>
#if !(defined(__WINDOWS__) || defined(_WIN32))
#include <dirent.h>
#endif

#include <cstdio>
#include <string>
#include <fstream>
#include <sys/stat.h>

static bool checkExt(std::string *str, std::string ext) {
	if (!ext.empty())
	{
		size_t pos = str->find_last_of(".");
		if (str->substr(pos+1) != ext || pos == std::string::npos)
			return false;
	}
	return true;
}

#if defined(_WIN32)
HANDLE FindFirstFile_Fixed( std::string dir, WIN32_FIND_DATAA *fd )
{
	return FindFirstFileA( dir.c_str(), fd );
}
#endif

std::vector<std::string> FileManager::GetDirectoryListing(string dir, string ext)
{
	vector<string> files;
	
#if defined(_WIN32)
	WIN32_FIND_DATAA fd;
	
	HANDLE hFind;
	if ( ext.empty() )
		hFind = FindFirstFile_Fixed( dir+"/*", &fd );
	else
		hFind = FindFirstFile_Fixed( dir+"/*."+ext, &fd );
	
	if ( hFind != INVALID_HANDLE_VALUE )
	{
		do {
			if( !strcmp(fd.cFileName, ".") || !strcmp(fd.cFileName, "..") )
				continue;
			
			files.push_back( std::string(fd.cFileName) );
		} while( FindNextFileA( hFind, &fd ) );
		FindClose( hFind );
	}
#else
	DIR *dp;
	struct dirent *dirp;
	
	if((dp  = opendir(dir.c_str())) == NULL)
	{
		LOG->Warn("Error (%d) opening %s", errno, dir.c_str());
		return files;
	}
	
	while ((dirp = readdir(dp)) != NULL)
	{
		string str = dirp->d_name;
		if (!checkExt(&str, ext))
			continue;
		files.push_back(str);
	}
	
	closedir(dp);
#endif
	
	return files;
}

bool FileManager::FileExists(string _file, string ext)
{
	struct stat stFileInfo;
	int iStat;
	iStat = stat(_file.c_str(),&stFileInfo);
	
	if (iStat == 0 && checkExt(&_file, ext))
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

string FileManager::GetPath(string _path)
{
//	if (_path[0] != '/' && Preferences != NULL)
//		_path = Log->SPrint("Themes/%s/%s",
//							Preferences->GetValue("Game", "CurrentTheme"),
//							_path.c_str());
//	else if (Preferences != NULL) // prevent "references.ini" and such.
//		_path = _path.substr(1);
	
	return GetWorkingDirectory() + _path;
}

string FileManager::GetFileContents(string _path)
{
	string out, buf;
	ifstream file(_path.c_str());
	
	if (!file.is_open())
	{
		LOG->Warn("Error opening %s for writing", _path.c_str());
		return string();
	}
	
	while (!file.eof())
	{
		getline(file, buf);
		out += buf;
		out += "\n";
	}
	
	return out;
}

/**
 * Colby Klein (c) 2011
 * Licensed under the terms of the MIT license. See License.txt.
 */
