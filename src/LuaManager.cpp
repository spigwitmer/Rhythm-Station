#include <cstring>
#include "LuaManager.h"
#include "FileManager.h"
#include <GL/glfw3.h>
#include <iostream>
#include <sstream>
#include <ostream>

LuaManager* Lua = NULL;

void LuaManager::PushInteger(std::string name, int value) {
	lua_pushinteger(L, value);
	lua_setglobal(L, name.c_str());
}

void LuaManager::PushString(std::string name, std::string value) {
	lua_pushstring(L, value.c_str());
	lua_setglobal(L, name.c_str());
}

void LuaManager::PushNumber(std::string name, double value) {
	lua_pushnumber(L, value);
	lua_setglobal(L, name.c_str());
}

int luafunc(lua_State *L) {
	char buf[16] = "";
	sprintf(buf, "%0.3f", glfwGetTime());

	std::ostringstream out;
	out << "[" << buf << "]";

	std::string tmp = out.str();
	lua_pushstring(L, tmp.c_str());

	return 1;
}

LuaManager::LuaManager() {
	L = lua_open();
	luaL_openlibs(L);
	lua_register(L, "TimeStamp", luafunc);
	PushInteger("ScreenWidth", 854);
	PushInteger("ScreenHeight", 480);
	PushString("Version", "Rhythm Station 0.03 [dev]");

	std::string file = File->GetFile("init.lua");
	file = File->GetFileContents(file);
	luaL_dostring(L, file.c_str());
}

LuaManager::~LuaManager() {
	lua_close(L);
}
