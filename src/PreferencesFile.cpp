#include <string>
#include "Logger.h"
#include "PreferencesFile.h"

PreferencesFile *Preferences = NULL;

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
	this->SetValue("Graphics", "WindowWidth", 854);
	this->SetValue("Graphics", "WindowHeight", 480);
	this->SetValue("Graphics", "VSync", true);
	this->SetValue("Graphics", "FullScreen", false);
	this->SetValue("Graphics", "AntiAliasing", true);
	this->SetValue("Graphics", "AntiAliasingSamples", 4);
}
