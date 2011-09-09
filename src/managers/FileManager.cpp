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
#include <glsw.h>

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

static bool checkExt(std::string *str, std::string ext) {
	if (!ext.empty())
	{
		size_t pos = str->find_last_of(".");
		if (str->substr(pos+1) != ext || pos == string::npos)
			return false;
	}
	return true;
}

#if defined(_WIN32)
HANDLE FindFirstFile_Fixed(string dir, WIN32_FIND_DATAA *fd)
{
	return FindFirstFileA(dir.c_str(), fd);
}
#endif

vector<string> FileManager::GetDirectoryListing(string dir, string ext)
{
	vector<string> files;
	
#if defined(_WIN32) // Windows
	WIN32_FIND_DATAA fd;
	
	HANDLE hFind;
	if (ext.empty())
		hFind = FindFirstFile_Fixed(dir+"/*", &fd);
	else
		hFind = FindFirstFile_Fixed(dir+"/*."+ext, &fd);
	
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (!strcmp(fd.cFileName, ".") || !strcmp(fd.cFileName, ".."))
				continue;
			
			files.push_back(string(fd.cFileName));
		}
		while (FindNextFileA(hFind, &fd));
		FindClose(hFind);
	}
#else // Unix
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
		
		// "." and ".." are useless - we don't ever need them.
		if (str == "." || str == "..")
			continue;

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

// this will probably be removed later.
string FileManager::GetWorkingDirectory()
{
	char path[1024] = "";
	
	if (getcwd(path, 1024) != NULL)
		return string(path) + "/";
	else
		return string("./");
}

// TODO: VFS
string FileManager::GetPath(string _path)
{
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
