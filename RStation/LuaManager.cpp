#include "LuaManager.h"
#include "FileManager.h"
#include "Logger.h"

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include <luabind/luabind.hpp>

lua_State *L;

void lua_testLoad()
{
	L = lua_open();
//	luaL_openlibs(L);

	luaopen_base(L);
	luaopen_table(L);
	luaopen_string(L);
	luaopen_math(L);
	luabind::open(L);

	using namespace FileManager;
	std::string file = GetFile("Test.lua");
	if(FileExists(file))
		luaL_dofile(L, file.c_str());
	else
		Log->Print("Init Lua file not found.");


//	lua_close(L);
}
