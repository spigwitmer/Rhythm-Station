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
#include "utils/PreferencesFile.h"

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

std::vector<std::string> FileManager::GetDirectoryListing(std::string dir, std::string ext)
{
	std::vector<std::string> files;

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
		Log->Print("Error (%d) opening %s", errno, dir.c_str());
		return files;
	}
	
	while ((dirp = readdir(dp)) != NULL)
	{
		std::string str = dirp->d_name;
		if (!checkExt(&str, ext))
			continue;
		files.push_back(str);
	}
	
	closedir(dp);
#endif
	
	return files;
}

bool FileManager::FileExists(std::string _file, std::string ext)
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

std::string FileManager::GetFile(std::string _path)
{
	if (_path[0] != '/' && Preferences != NULL)
		_path = Log->SPrint("Themes/%s/%s",
			Preferences->GetValue("Game", "CurrentTheme"),
			_path.c_str());
	else if (Preferences != NULL) // prevent "references.ini" and such.
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


/**
 * @file
 * @author Colby Klein (c) 2011
 * @section LICENSE
 * 
 * All rights reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, and/or sell copies of the Software, and to permit persons to
 * whom the Software is furnished to do so, provided that the above
 * copyright notice(s) and this permission notice appear in all copies of
 * the Software and that both the above copyright notice(s) and this
 * permission notice appear in supporting documentation.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF
 * THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS
 * INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
