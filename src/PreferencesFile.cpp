#include <string>
#include "Logger.h"
#include "PreferencesFile.h"

PreferencesFile* Preferences = NULL;

PreferencesFile::PreferencesFile(std::string _path) : INIFile(_path)
{
	if (!this->Load())
	{
		Log->Print("Setting default preferences..");
		this->SetDefaults();
	}
}

PreferencesFile::~PreferencesFile()
{
	this->Save();
}

void PreferencesFile::SetDefaults()
{
	this->SetValue("Graphics","windowWidth",854);
	this->SetValue("Graphics","windowHeight",480);
}
