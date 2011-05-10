#include "ScreenManager.h"
#include "screens/Screen.h"
#include "RStation.h"

using namespace std;

MakeScreenMap *ScreenManager::GetMap()
{
	static MakeScreenMap g_ScreenMap;
	return &g_ScreenMap;
}

void ScreenManager::Update(double time)
{
	double delta = time - m_LastUpdate;
	m_LastUpdate = time;
	
	vector<Screen*>::iterator it = m_vScreenStack.begin();
	for (; it != m_vScreenStack.end(); it++)
		(*it)->Update(delta);
}

void ScreenManager::Draw()
{
	vector<Screen*>::iterator it = m_vScreenStack.begin();
	for (; it != m_vScreenStack.end(); it++)
		(*it)->Draw();
}

void ScreenManager::PushScreen(string sName)
{
	MakeScreenMap::iterator it = GetMap()->find(sName);

	if (it == GetMap()->end())
		return;

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
 * @file
 * @author Colby Klein (c) 2011
 * @section LICENSE
 * 
 * This program is licensed under the terms of the MIT license.
 * The full text can be found in License.txt.
 */
