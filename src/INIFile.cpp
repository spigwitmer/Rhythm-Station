#include <string>
#include "FileManager.h"
#include "Logger.h"
#include "INIFile.h"

INIFile::INIFile(std::string _path)
{
	this->relpath = _path;
	this->path = FileManager::GetFile(_path);
	this->backend.SetUnicode();
}

bool INIFile::Load()
{
	if (!FileManager::FileExists(this->path))
	{
		Log->Print("Ini-file '%s' doesn't exists.",this->relpath.c_str());
		return false;
	}
	
	if (this->backend.LoadFile(this->path.c_str()) < 0)
	{
		Log->Print("Ini-file '%s' couldn't be loaded.",this->relpath.c_str());
		return false;
	}
	
	return true;
}

bool INIFile::Save()
{
	if (this->backend.SaveFile(this->path.c_str()) < 0)
	{
		Log->Print("Ini-file '%s' couldn't be written.",this->relpath.c_str());
		return false;
	}
	
	return true;
}

const char *INIFile::GetValue(const char *section,
                              const char *key,
                              const char *default_value)
{
	return this->backend.GetValue(section,key,default_value);
}

long INIFile::GetLongValue(const char *section,
                           const char *key,
                           long default_value)
{
	return this->backend.GetLongValue(section,key,default_value);
}

bool INIFile::GetBoolValue(const char *section,
                           const char *key,
                           bool default_value)
{
	return this->backend.GetBoolValue(section,key,default_value);
}

bool INIFile::SetValue(const char *section,
                       const char *key,
                       const char *value,
                       const char *comment)
{
	if (this->backend.SetValue(section,key,value,comment) < 0)
		return false;
		
	return true;
}

bool INIFile::SetValue(const char *section,
                       const char *key,
                       long value,
                       const char *comment)
{
	if (this->backend.SetLongValue(section,key,value,comment) < 0)
		return false;
		
	return true;
}

bool INIFile::SetValue(const char *section,
                       const char *key,
                       int value,
                       const char *comment)
{
	if (this->backend.SetLongValue(section,key,(long) value,comment) < 0)
		return false;
		
	return true;
}

bool INIFile::SetValue(const char *section,
                       const char *key,
                       bool value,
                       const char *comment)
{
	if (this->backend.SetBoolValue(section,key,value,comment) < 0)
		return false;
		
	return true;
}


/**
 * @file
 * @author Jarno Purontakanen (c) 2011
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
