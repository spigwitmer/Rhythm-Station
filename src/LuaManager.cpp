#include <cstring>
#include "LuaManager.h"
#include "FileManager.h"
#include "Logger.h"

LuaManager* Lua = NULL;

LuaManager::LuaManager()
{
	L = lua_open();
	luaL_openlibs(L);
}

void LuaManager::Run(const char* path)
{
	// Fix path and get contents
	std::string file = FileManager::GetFile(path);
	file = FileManager::GetFileContents(file);
	
	// Load into Lua, run.
	luaL_loadstring(L, file.c_str());
	int status = lua_pcall(L, 0, LUA_MULTRET, 0);
	if (!status)
		return;
	
	// If we're here, something is wrong.
	Log->Print("***** Lua runtime error in %s *****", path);
	Log->Print(lua_tostring(L, -1));
}

LuaManager::~LuaManager()
{
	lua_close(L);
}
