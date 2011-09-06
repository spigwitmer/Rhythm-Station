#include <GL3/gl3w.h>
#include <GL/glfw3.h>
#include "ScreenManager.h"
#include "screens/Screen.h"
#include "RStation.h"
#include "utils/Logger.h"

using namespace std;

MakeScreenMap *ScreenManager::GetMap()
{
	static MakeScreenMap g_ScreenMap;
	return &g_ScreenMap;
}

ScreenManager::ScreenManager()
{
	m_LastUpdateRounded = 0;
	m_LastUpdate = 0.0;
}

void ScreenManager::Update()
{
	const size_t NUM_FRAMES = 60*5;
	double time = glfwGetTime();
	double delta = time - m_LastUpdate;
	double avg = 0.0;
	
	// Calculate Average FPS.
	m_Times.push_back(delta);
	
	if (m_Times.size() > NUM_FRAMES)
		m_Times.pop_front();
	
	for (std::deque<double>::iterator i = m_Times.begin(); i != m_Times.end(); i++)
		avg += *i;
	
	if (int(time) % 1 == 0 && avg > 0.0001)
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
	MakeScreenMap::iterator it = GetMap()->find(sName);

	if (it == GetMap()->end())
	{
		LOG->Warn("Screen \"%s\" not found!", sName.c_str());
		return;
	}

	m_vScreenStack.push_back(it->second(sName));
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
}

/**
 * Colby Klein, Matt Vandermeulen (c) 2011
 * Licensed under the terms of the MIT license. See License.txt.
 */
