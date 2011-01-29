#include <cstring>
#include "LuaManager.h"
#include "FileManager.h"
#include "Logger.h"
#include <SLB/SLB.hpp>
#include "Object.h"
#include "Sprite.h"
#include "Sound.h"

LuaManager *Lua = NULL;

LuaManager::LuaManager()
{
	L = lua_open();
	luaL_openlibs(L);
	
	// Register stuff with Lua.
	SLB::Manager::getInstance().registerSLB(L);
	Object_Binding();
	Sprite_Binding();
	Sound_Binding();
}

LuaManager::~LuaManager()
{
	lua_close(L);
}

void LuaManager::Run(const char *path)
{
	// Fix path and get contents
	std::string file = FileManager::GetFile(path);
	file = FileManager::GetFileContents(file);
	file = "SLB.using(SLB)\n" + file;
	
	// Load into Lua, run.
	luaL_loadstring(L, file.c_str());
	int status = lua_pcall(L, 0, LUA_MULTRET, 0);
	
	if (!status)
		return;
		
	// If we're here, something is wrong.
	Log->Print("***** Lua runtime error in %s *****", path);
	Log->Print(lua_tostring(L, -1));
	
	// pop error message from the stack.
	lua_pop(L, 1);
}
