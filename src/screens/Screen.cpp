#include "screens/Screen.h"
#include "utils/Logger.h"
#include "managers/LuaManager.h"
#include "managers/InputManager.h"
#include "utils/Timer.h"

using namespace std;

REGISTER_SCREEN(Screen);

Screen::Screen(string sName) :
	m_error(false),
	m_Lua(NULL),
	m_name(sName),
	m_projection(1.0)
{
	SubscribeTo("ScreenChanged");
	SubscribeTo("Input");
}

Screen::~Screen()
{
	LOG->Info("Deleted screen \"%s\".", m_name.c_str());
}

void Screen::Reset()
{
	m_error = false;
	m_projection = glm::mat4(1.0);
	Init();
}

// Use Init() so that the timer works properly!
void Screen::InitInternal()
{
	lua_State *L = m_Lua->Get();
	lua_getglobal(L, m_name.c_str());

	if (!lua_istable(L, -1))
		return;

	// note: lua_next needs the stack to be: -1 = key(nil), -2 = table
	lua_pushnil(L);

	while (lua_next(L, -2)) {
		// -1 = value, -2 = key, -3 = table
		//if (lua_isnumber(L, -2))
		//	LOG->Info("oh hey %d=%d", (int)lua_tonumber(L, -2), (int)lua_tonumber(L, -1));

		lua_pop(L, 1);
	}

	Init();
}

void Screen::Init()
{
}

void Screen::HandleMessage(const Message &msg)
{
	InputManager *input = (InputManager*)msg.data;
	if (msg.name == "Input") {
		while (input->GetNextInput()) {
			// TODO
		}
	}

	LOG->Info("Got message: %s", msg.name.c_str());
}

void Screen::Push(Actor* obj)
{
	m_objects.push_back(obj);
}

void Screen::UpdateInternal(double delta)
{
}

void Screen::Update(double delta)
{
	if (!m_error)
	{
		// (Update stuff)
		lua_State *L = m_Lua->Get();
		lua_getglobal(L, m_name.c_str());
		lua_getfield(L, -1, "Update");
		if (lua_isfunction(L, -1))
		{
			int err = lua_pcall(L, 0, 0, 0);
			if (err)
			{
				m_error = true;
				LOG->Warn("%s", lua_tostring(L, -1));
				lua_pop(L, 1);
			}
		}
	}

	UpdateInternal(delta);
}

void Screen::Draw()
{
}

/**
 * Colby Klein, Matt Vandermeulen (c) 2011
 * Licensed under the terms of the MIT license. See License.txt.
 */
