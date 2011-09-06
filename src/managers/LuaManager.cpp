#include "LuaManager.h"
#include <vector>

using namespace std;

LuaManager::LuaManager(FileManager &f)
{
	m_Lua = lua_open();
	m_FileMan = &f;
}

void LuaManager::Bind(string s)
{
	vector<string> files = m_FileMan->GetDirectoryListing(s);
	if (files.empty())
		return;
	
	vector<string>::iterator it;
	for (it = files.begin(); it != files.end(); it++)
		luaL_loadfile(m_Lua, it->c_str());
}

void LuaManager::Init()
{
	
}

/**
 * Colby Klein (c) 2011
 * Licensed under the terms of the MIT license. See License.txt.
 */
