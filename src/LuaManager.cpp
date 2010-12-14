#include <cstring>
#include "LuaManager.h"
#include "FileManager.h"
#include "RenderManager.h"
#include <GL/glfw3.h>
#include <iostream>
#include <sstream>
#include <ostream>
#include <SLB/SLB.hpp>
#include "Object.h"
#include "Type.h"
#include "Sound.h"

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
	SLB::Manager::getInstance().registerSLB(L);

	// todo: automatically register classes
	Type_Binding();
	Object_Binding();
	Sound_Binding();

	lua_register(L, "TimeStamp", luafunc);
	PushInteger("ScreenWidth", Renderer->ScreenWidth);
	PushInteger("ScreenHeight", Renderer->ScreenHeight);
	PushString("ProductID", "Rhythm Station");
	PushNumber("Version", 0.03);
}

void LuaManager::Start() {
	std::string file = File->GetFile("init.lua");
	file = "SLB.using(SLB)\n" + File->GetFileContents(file);
	luaL_dostring(L, file.c_str());
}

LuaManager::~LuaManager() {
	lua_close(L);
}
