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
	
	// Note: VSync doesn't work properly in windowed mode.
	this->SetValue("Graphics", "VSync", false);
	this->SetValue("Graphics", "FullScreen", false);
	this->SetValue("Graphics", "AntiAliasing", false);
	this->SetValue("Graphics", "AntiAliasingSamples", 4);
	
	this->SetValue("Game", "CurrentTheme", "default");
}


/**
 * @file
 * @author Colby Klein, Jarno Purontakanen (c) 2011
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
