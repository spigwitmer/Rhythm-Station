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

bool LuaManager::CheckError(int status)
{
	if (status) {		
		// If we're here, something is wrong.
		Log->Print("[ERROR] Lua error! -> %s", lua_tostring(L, -1));
		
		// pop error message from the stack.
		lua_pop(L, 1);
		
		return true;
	}
	return false;
}

void LuaManager::Run()
{
	using namespace std;
	using namespace FileManager;
	
	int status = 0;
	vector<string>::iterator it;
	vector<string> vScripts;
	
	// Get all scripts in current theme folder.	
	Log->Print("Searching for Lua scripts...");
	vScripts = GetDirectoryListing(GetFile(""), "lua");
	
	// Sort scripts. We need this in order.
	sort(vScripts.begin(), vScripts.end());
	
	for (it = vScripts.begin(); it != vScripts.end(); it++)
		Log->Print("-> Found \"%s\"", (*it).c_str());
	
	// Fix path and get contents
	luaL_loadstring(L, "SLB.using(SLB)\n");
	status = lua_pcall(L, 0, LUA_MULTRET, 0);
	if (CheckError(status))
		return;
	
	Log->Print("Running all scripts...");
	
	for (it = vScripts.begin(); it != vScripts.end(); it++)
	{
		Log->Print("-> Running \"%s\"", (*it).c_str());
		
		luaL_loadfile(L, GetFile(*it).c_str());
		status = lua_pcall(L, 0, LUA_MULTRET, 0);
		if (CheckError(status))
			break;
	}
}
