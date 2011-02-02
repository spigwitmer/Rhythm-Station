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
	this->SetValue("Graphics", "WindowWidth", 1024);
	this->SetValue("Graphics", "WindowHeight", 640);
	
	// Note: VSync doesn't work properly in windowed mode, at least on OS X.
	this->SetValue("Graphics", "VSync", false);
	this->SetValue("Graphics", "FullScreen", false);
	
	this->SetValue("Graphics", "AntiAliasing", false);
	this->SetValue("Graphics", "AntiAliasingSamples", 4);
}
