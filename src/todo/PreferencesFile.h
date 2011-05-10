#ifndef _PREFERENCES_FILE_H_
#define _PREFERENCES_FILE_H_

#include <string>
#include "INIFile.h"

class PreferencesFile : public INIFile
{
public:
	PreferencesFile(std::string _path);
	~PreferencesFile();
private:
	void SetDefaults();
};

extern PreferencesFile *Preferences;

#endif
