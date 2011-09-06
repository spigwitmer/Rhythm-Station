#pragma once

#include "FileManager.h"
#include <string>

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

class LuaManager
{
public:
	LuaManager(FileManager &f);
	void Init();
	void Bind(std::string s);
	
private:
	FileManager *m_FileMan;
	lua_State *m_Lua;
};

/**
 * Colby Klein (c) 2011
 * Licensed under the terms of the MIT license. See License.txt.
 */
