#include <cstring>
#include "LuaManager.h"
#include "FileManager.h"
#include "Logger.h"
#include <SLB/SLB.hpp>
#include "Object.h"
#include "Sprite.h"
#include "Sound.h"
#include "NoteField.h"
#include <algorithm>

static const luaL_Reg libs[] =
{
	{ "base",	luaopen_base },
	{ "table",	luaopen_table },
	{ "string",	luaopen_string },
	{ "math",	luaopen_math },
	{ "debug",	luaopen_debug },
/*
	{ "io",	luaopen_io },
	{ "os",	luaopen_os },
	{ "package", luaopen_package },
*/
	{ NULL, NULL }
};

// load the list of libs
static void open_libs(lua_State *L)
{
	const luaL_reg *lib = libs;
	for ( ; lib->func; lib++)
	{
		lib->func(L);		// open library
		lua_settop(L, 0);	// discard any results
	}
}

LuaManager::LuaManager()
{
	L = lua_open();
	open_libs(L);
	
	// Register stuff with Lua.
	SLB::Manager::getInstance().registerSLB(L);
	Object_Binding();
	Sprite_Binding();
	Sound_Binding();
	NoteField_Binding();	
}

LuaManager::~LuaManager()
{
	lua_close(L);
}

void LuaManager::Run()
{
	Log->Print("Searching for Lua scripts...");
	// Get all scripts in current theme folder.
	std::vector<std::string> vScripts = FileManager::GetDirectoryListing(FileManager::GetFile(""), "lua");
	
	// Sort them. We need this in order.
	std::sort(vScripts.begin(), vScripts.end());

	std::vector<std::string>::iterator it;
	for (it = vScripts.begin(); it != vScripts.end(); it++)
	{
		Log->Print("-> Found \"%s\"", (*it).c_str());
	}

	// Fix path and get contents
	std::string file = "SLB.using(SLB)\n";
	luaL_loadstring(L, file.c_str());
	int status = lua_pcall(L, 0, LUA_MULTRET, 0);
	if (status)
	{
		Log->Print("Irrecoverable Lua error. Aborting.");
		return;
	}
	
	Log->Print("Running all scripts...");
	
	for (it = vScripts.begin(); it != vScripts.end(); it++)
	{
		Log->Print("-> Running \"%s\"", (*it).c_str());
		file = FileManager::GetFile((*it));
		luaL_loadfile(L, file.c_str());
		
		status = lua_pcall(L, 0, LUA_MULTRET, 0);
		if (status)
		{
			// If we're here, something is wrong.
			Log->Print("[ERROR] Lua error! Please check script.");
			Log->Print(lua_tostring(L, -1));
			
			// pop error message from the stack.
			lua_pop(L, 1);
			
			break;
		}
	}
	
	Log->Print("Script execution complete.");
}
