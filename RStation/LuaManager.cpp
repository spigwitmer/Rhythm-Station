#include "LuaManager.h"
#include "FileManager.h"

extern "C" {
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

	luaL_dofile(L,FileManager::GetFile("Test.lua").c_str());

//	lua_close(L);
}
