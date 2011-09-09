#include "LuaManager.h"
#include <vector>

#include "utils/Logger.h"

using namespace std;

LuaManager::LuaManager(FileManager &f)
{
	m_Lua = lua_open();
	m_FileMan = &f;
	
	luaL_openlibs(m_Lua);
}

void LuaManager::Init(string s)
{
	vector<string> files = m_FileMan->GetDirectoryListing(s);
	if (files.empty())
		return;
	
	vector<string>::iterator it;
	for (it = files.begin(); it != files.end(); it++)
	{
		string str = s + "/" + (*it);
		LOG->Info("Running \"%s\".", str.c_str());
		luaL_dofile(m_Lua, str.c_str());
	}
}

lua_State* LuaManager::Get()
{
	return m_Lua;
}

/**
 * Colby Klein (c) 2011
 * Licensed under the terms of the MIT license. See License.txt.
 */
