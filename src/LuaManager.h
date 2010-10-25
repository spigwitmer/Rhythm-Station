#ifndef _LUA_MANAGER_H_
#define _LUA_MANAGER_H_

#include <string>

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

class LuaManager {
public:
	LuaManager();
	virtual ~LuaManager();
	void PushInteger(std::string name, int value);
	void PushString(std::string name, std::string value);
	void PushNumber(std::string name, double value);

private:
	lua_State *L;
};

extern LuaManager* Lua;

#endif
