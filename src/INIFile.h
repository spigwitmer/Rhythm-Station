#ifndef _INI_FILE_H_
#define _INI_FILE_H_

#include <string>

#ifdef _WIN32
	#pragma warning(disable: 4786)
#endif

#ifndef _WIN32
	#include <unistd.h>
#endif

#include "SimpleIni.h"

class INIFile
{
public:
	INIFile(std::string _path);
	bool Load();
	bool Save();

	const char* GetValue(const char* section,
		const char* key,
		const char* default_value = NULL);
	long GetLongValue(const char* section,
		const char* key,
		long default_value = 0);
	bool GetBoolValue(const char* section,
		const char* key,
		bool default_value = false);

	bool SetValue(const char* section,
		const char* key,
		const char* value,
		const char* comment = NULL);
	bool SetValue(const char* section,
		const char* key,
		long value,
		const char* comment = NULL);
	bool SetValue(const char* section,
		const char* key,
		int value,
		const char* comment = NULL);
	bool SetValue(const char* section,
		const char* key,
		bool value,
		const char* comment = NULL);
private:
	std::string relpath;
	std::string path;
	CSimpleIniCaseA backend;
};

#endif
