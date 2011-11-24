#include <GL3/gl3w.h>
#include <GL/glfw3.h>
#include "ScreenManager.h"
#include "screens/Screen.h"
#include "RStation.h"
#include "utils/Logger.h"
#include "utils/Timer.h"

using namespace std;

MakeScreenMap *ScreenManager::GetMap()
{
	static MakeScreenMap g_ScreenMap;
	return &g_ScreenMap;
}

ScreenManager::ScreenManager(LuaManager &lua, FileManager &fm)
{
	m_LuaMan = &lua;
	m_FileMan = &fm;
	m_LastUpdateRounded = 0;
	m_LastUpdate = 0.0;
}

ScreenManager::~ScreenManager()
{
	ClearStack();
}

void ScreenManager::Update()
{
	const size_t NUM_FRAMES = 60 * 5;
	double time = glfwGetTime();
	double delta = time - m_LastUpdate;
	double avg = 0.0;

	// Calculate Average FPS.
	m_Times.push_back(delta);

	if (m_Times.size() > NUM_FRAMES)
		m_Times.pop_front();

	// TODO: also get the min and max.
	for (std::deque<double>::iterator i = m_Times.begin(); i != m_Times.end(); i++)
		avg += *i;

	if (int(time) % 1 == 0 && avg > 0.001)
	{
		if (m_LastUpdateRounded != int(time))
			LOG->Info("Avg. FPS: %0.0f",
					  glm::ceil(1.0 / (avg / m_Times.size())));
		m_LastUpdateRounded = int(time);
	}

	m_LastUpdate = time;

	vector<Screen*>::iterator it = m_vScreenStack.begin();
	for (; it != m_vScreenStack.end(); it++)
		(*it)->Update(delta);
}

void ScreenManager::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	vector<Screen*>::iterator it = m_vScreenStack.begin();
	for (; it != m_vScreenStack.end(); it++)
		(*it)->Draw();
}

void ScreenManager::PushScreen(string sName)
{
	lua_State *L = m_LuaMan->Get();

	// Make sure we've got a table to match the given name.
	lua_getglobal(L, sName.c_str());
	if (!lua_istable(L, -1))
	{
		LOG->Warn("No screen named \"%s\" found.", sName.c_str());
		return;
	}

	// We need the Class field, so that we know what to instantiate.
	lua_getfield(L, -1, "Class");
	if (lua_isstring(L, -1)) {
		string sClass = lua_tostring(L, -1);

		MakeScreenMap::iterator it = GetMap()->find(sClass);

		if (it == GetMap()->end())
		{
			LOG->Warn("Invalid screen class: \"%s\".", sClass.c_str());
			return;
		}		
		LOG->Info("Creating screen \"%s\" (Class=\"%s\").", sName.c_str(), sClass.c_str());

		Screen* scr = it->second(sName);
		scr->SetLuaManager(m_LuaMan);
		scr->SetFileManager(m_FileMan);

		Timer t;
		scr->InitInternal();
		LOG->Info("Loaded \"%s\" in %fs", sName.c_str(), t.Ago());

		m_vScreenStack.push_back(scr);
	}
	else
	{
		LOG->Warn("%s has no class! Skipping.", sName.c_str());
	}
}

void ScreenManager::PopScreen()
{
	SAFE_DELETE(m_vScreenStack.back());

	m_vScreenStack.pop_back();
}

void ScreenManager::ClearStack()
{
	while (!m_vScreenStack.empty())
		PopScreen();

	LOG->Info("Cleared screen stack.");
}

/**
 * Colby Klein, Matt Vandermeulen (c) 2011
 * Licensed under the terms of the MIT license. See License.txt.
 */
