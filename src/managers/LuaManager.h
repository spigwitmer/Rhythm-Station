#pragma once

#include "FileManager.h"
#include <string>

class LuaManager
{
public:
	LuaManager(FileManager &f);
	void Init();
	void Bind(std::string s);
};

/**
 * Colby Klein (c) 2011
 * Licensed under the terms of the MIT license. See License.txt.
 */
