#ifndef _LUA_MANAGER_H_
#define _LUA_MANAGER_H_

#include <string>
#include "Singleton.h"

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

class LuaManager : public Singleton<LuaManager>
{
public:
	LuaManager();
	virtual ~LuaManager();
	
	void Run();
	bool CheckError(int status);
	
	lua_State *L;
};

#endif
